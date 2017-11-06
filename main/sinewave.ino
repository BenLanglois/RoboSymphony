int sin_table[65];

void calculate_sin_table() {
    for (int i = 0; i <= 64; i++)
    if (i == 64) sin_table[i] = 127;
    else sin_table[i] = (int)(128 * sin(M_PI * i / 128));
}

int custom_sin(int theta) {
    assert(0 <= theta && theta < 256);
    if (0 <= theta && theta < 64) return 128 + sin_table[theta];
    else if (64 <= theta && theta < 128) return 128 + sin_table[128 - theta];
    else if (128 <= theta && theta < 192) return 128 - sin_table[theta - 128];
    else if (192 <= theta && theta < 256) return 128 - sin_table[256 - theta];
    assert(0);
}
