LOWEST_PRESSURE = 700
HIGHEST_PRESSURE = 1100

def main():
    print('constexpr int PRESSURE_LOOKUP[] = { ', end='')
    for p in range(LOWEST_PRESSURE, HIGHEST_PRESSURE):
        print(f'{int(44330 * (1 - pow(p * 100 / 101325, 1 / 5.255)))}, ', end='')
    print('};')

if __name__ == '__main__':
    main()
