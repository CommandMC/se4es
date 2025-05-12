import io
import os
import stat
import time
import sys

import requests
import re
import argparse
import zipfile
import shutil

import json

from urllib.parse import urlparse
from bs4 import BeautifulSoup

# TODO CHANGE HERE
tags_url = "https://ess.cs.uos.de/git/software/wsim/-/tags"
# TODO CHANGE HERE

updater_dir = os.path.dirname(__file__)
version_file = os.path.join(updater_dir, "wsim_version.txt")
extract_dir = os.path.join(updater_dir, "extract")
bin_dir = os.path.join(updater_dir, "bin")

parser = argparse.ArgumentParser()
parser.add_argument('--t', '--tags', type=str,
                    default="stable", help='List of wanted release types: ("pre", "alpha", "beta", "unstable", "stable")'
                                           ', divided by comma')
parser.add_argument('--p', '--peek', type=bool,
                    default=False, help='If true only check if new version if available, don\'t update automatic')
args = parser.parse_args()

def file_older(file, hours=12):
    file_time = os.path.getmtime(file)
    return (time.time() - file_time) > hours * 3600


if os.path.exists(version_file):
    with open(version_file, "r") as f:
        version = f.readline().replace("\n", "")
    if not file_older(version_file, hours=12) and args.p:
        exit(0)
else:
    version = "0.0"

print("[WSIM-Updater] Checking for updates...")

domain = urlparse(tags_url).netloc
try:
    text = requests.get(tags_url).text
except:
    print("[WSIM-Updater] Connection problem!")
    exit(0)

allowed_tag_starts = tuple(args.t.split(","))  # ("pre", "alpha", "beta", "unstable", "stable")
only_peek = args.p

bs = BeautifulSoup(text, features="html.parser")

for x in bs.find_all("div", {"class": "js-source-code-dropdown"}): # mld: This will break with the next gitlab update
    xj = json.loads(x["data-download-artifacts"])
    if "build" in xj[0]["text"]:
        # print(x)
        m = re.search(".*artifacts/(.+?)/download.*", xj[0]['path'])
        if m:
            if m.group(1).startswith(allowed_tag_starts):
                if m.group(1) != version:
                    if only_peek:
                        print("==================================================")
                        print("====          WSIM UPDATE AVAILABLE           ====")
                        print("====                                          ====")
                        print("==== {:>18s} >> {:<18s} ====".format(version, m.group(1)))
                        print("====                                          ====")
                        print("====      Please call 'make wsim-update'      ====")
                        print("==================================================")
                        exit(0)
                    # update / download file
                    response = requests.get("https://" + domain + xj[0]['path'])
                    zipDocument = zipfile.ZipFile(io.BytesIO(response.content))
                    wsim_src = zipDocument.extract('platforms/ez430chronos/wsim-ez430chronos', extract_dir)
                    wsnet_src = zipDocument.extract('utils/wsnet1/src/wsnet1', extract_dir)
                    wsim_dest = os.path.join(bin_dir, "wsim-ez430chronos")
                    wsnet_dest = os.path.join(bin_dir, "wsnet1")
                    os.makedirs(bin_dir, exist_ok=True)
                    os.replace(wsim_src, wsim_dest)
                    os.replace(wsnet_src, wsnet_dest)
                    os.chmod(wsim_dest, 0o755)
                    os.chmod(wsnet_dest, 0o755)
                    shutil.rmtree(extract_dir)

                    # update version:
                    with open(version_file, "w+") as f:
                        f.write(m.group(1))
                    print(f"UPDATE SUCCESSFUL! Now on version {m.group(1)}")
                    exit(0)
                # update file timestamp
                with open(version_file, "w+") as f:
                    f.write(version)
                exit(0)
