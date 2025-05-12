#include "device/plugbox.h"
#include "machine/simpliciti/simpliciti.h"
#include "machine/radio.h"

bool rxcallback;

extern "C" uint8_t callback_function(linkID_t lid) {
  rxcallback = true;
  return 0;
}

bool Radio::prologue() {
  /* Interrupt an SimpliciTI weiterreichen */
  rxcallback = false;
  MRFI_RadioIsr();
  return rxcallback;
}

void Radio::epilogue() {
  /* Angekommenes Paket signalisieren */
  packet_received.Semaphore::signal();
}

void Radio::plugin() {
  addr_t *localaddr;
  addr_t randomaddr;

  /* Knotenadresse aus Info-Flash verwenden */
  localaddr = (addr_t *)0x1800;

  if (localaddr->addr[0] == 0xff &&
      localaddr->addr[1] == 0xff &&
      localaddr->addr[2] == 0xff &&
      localaddr->addr[3] == 0xff) {
    /* Info-Flash ist leer, halbwegs zufaellige Adresse generieren */
    randomaddr.addr[0] = 0xe2;
    randomaddr.addr[1] = 0xfc;
    randomaddr.addr[2] = UCSCTL0_H;
    randomaddr.addr[3] = UCSCTL0_L;
    localaddr = &randomaddr;
  }

  /* Adresse setzen */
  SMPL_Ioctl(IOCTL_OBJ_ADDR, IOCTL_ACT_SET, localaddr);

  /* Interruptroutine anmelden - die Initialisierung sendet schon! */
  Plugbox::instance().assign(Plugbox::Radio, *this);

  /* Rueckgabewert ignoriert, da der eh nicht besonders informativ ist */
  SMPL_Init(callback_function);

  /* Radio sofort schlafen lassen */
  sleep();
}

unsigned long Radio::get_id() {
  union {
    addr_t linkaddr;
    unsigned long link32;
  } converter;

  /* Eingestellte(!) ID zurueckliefern */
  SMPL_Ioctl(IOCTL_OBJ_ADDR, IOCTL_ACT_GET, &converter.linkaddr);

  return converter.link32;
}

void Radio::enable_rx() {
  if (sleeping) { awake(); }
  rx_enabled = true;
  SMPL_Ioctl(IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_RXON, 0);
}

void Radio::disable_rx() {
  // Auf keinen Fall IOCTL_ACT_RADIO_RXIDLE mehrfach nutzen -> Hardware friert ein!
  if (!rx_enabled) 
    return;
  rx_enabled = false;
  SMPL_Ioctl(IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_RXIDLE, 0);
}

bool Radio::send(unsigned char link, void *data, unsigned char length) {
  bool was_sleeping = sleeping;
  if (sleeping) { awake(); }

  smplStatus_t status;

  status = SMPL_SendOpt(link, (uint8_t *)data, length, SMPL_TXOPTION_NONE);
  
  if (was_sleeping) { sleep(); }
  
  if (status == SMPL_SUCCESS)
    return true;
  else
    return false;
}

bool Radio::receive(unsigned char link, void *data, unsigned char *lengthptr) {
  smplStatus_t status;

  status = SMPL_Receive(link, (uint8_t *)data, lengthptr);
  if (status == SMPL_SUCCESS)
    return true;
  else
    return false;
}

bool Radio::send_broadcast(void *data, unsigned char length) {
  return send(SMPL_LINKID_USER_UUD, data, length);
}

bool Radio::receive_broadcast(void *data, unsigned char *lengthptr) {
  return receive(SMPL_LINKID_USER_UUD, data, lengthptr);
}

bool Radio::link(unsigned char &link) {
  bool status = SMPL_Link(&link);
  return status == SMPL_SUCCESS;
}

bool Radio::link_listen(unsigned char &link) {
  bool status = SMPL_LinkListen(&link);
  return status == SMPL_SUCCESS;
}

void Radio::sleep() {
  if (sleeping) { return; }
  SMPL_Ioctl(IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_SLEEP, 0);
  sleeping = true;
}

void Radio::awake() {
  SMPL_Ioctl(IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_AWAKE, 0);
  sleeping = false;
}

