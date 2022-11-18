#include <iostream>
using namespace std;

uint64_t buffer[10];
uint8_t C[8];

inline uint16_t GetDataAs16b(uint64_t& buf, int index) {
    return *((uint16_t*)&buf + index);
}

uint64_t FloatMult(uint64_t a, uint64_t b) {
    int cycle_cnt = 0;
    uint64_t temp64;
    uint8_t* temp_ptr8;
    uint16_t* temp_ptr16;
    buffer[0] = a;
    buffer[1] = b;
    buffer[2] = buffer[0] & (~0L << 40); cycle_cnt++;
    buffer[3] = buffer[1] & (~0L << 40); cycle_cnt++;
    buffer[3] = buffer[3] >> 24; cycle_cnt+=2;
    // step 1: mantissas multiplication
    for (int i = 0; i < 24; i++) {
        temp64 = buffer[2] & (1L << 63);
        buffer[2] = buffer[2] << 1; cycle_cnt++;
        if (temp64 > 0) {
            buffer[2] = buffer[2] + buffer[3];
        }
        cycle_cnt++;
    }
    {
        uint64_t t1 = buffer[0] >> 40, t2 = buffer[1] >> 40;
        uint64_t t3 = (t1 * t2) << 16;
        printf("a mantissa = 0x%lx, b mantissa = 0x%lx, result = 0x%lx\n",
            buffer[0], buffer[1], buffer[2]);
        if (t3 != buffer[2]) { exit(-1); }
    }
    // step 2: normalization bit
    temp64 = buffer[2] & (1L << 63);
    if (temp64 == 0L) {
        buffer[2] = buffer[2] + buffer[2];
    }
    cycle_cnt++;
    for (int i = 0; i < 8; i++) {
        C[i] = temp64 >> 63;
    }
    buffer[2] = buffer[2] & (~0L << 40); cycle_cnt++;

    // step 3: write back C
    temp_ptr8 = (uint8_t*)&buffer[4];
    temp_ptr8 += 2;
    for (int i = 0; i < 8; i++) {
        *temp_ptr8 = *temp_ptr8 | (C[2] << i);
    }
    cycle_cnt+=2; // Set mask bits

    cycle_cnt++;
    temp_ptr16 = ((uint16_t*)&buffer[2]) + 1;
    buffer[5] = 0x81L << 23;
    // step 4: exponential addition. Totally 3 operations.
    *temp_ptr16 = GetDataAs16b(buffer[0], 1) + GetDataAs16b(buffer[1], 1);
    cycle_cnt++;
    *temp_ptr16 = GetDataAs16b(buffer[4], 1) + *temp_ptr16;
    cycle_cnt++;
    temp64 = GetDataAs16b(buffer[5], 1) + *temp_ptr16;
    if (temp64 & 0x10000) {
        temp64 = temp64 | ~0;
    }
    *temp_ptr16 = temp64 & 0xFFFF;
    cycle_cnt++;
    temp64 = *temp_ptr16 & 0x8000;
    if (temp64 == 0L) {
        *temp_ptr16 = 0;
    }
    cycle_cnt++;
    *temp_ptr16 = *temp_ptr16 & (0xFF << 7); cycle_cnt++;

    // detecting 0
    buffer[6] = ~0L;
    temp64 = GetDataAs16b(buffer[6], 1) + GetDataAs16b(buffer[0], 1);
    if ((temp64 & (1<<16)) == 0) {
        *temp_ptr16 = 0;
    }
    temp64 = GetDataAs16b(buffer[6], 1) + GetDataAs16b(buffer[1], 1);
    if ((temp64 & (1<<16)) == 0) {
        *temp_ptr16 = 0;
    }
    cycle_cnt+=2;

    cycle_cnt++;
    temp_ptr16 = ((uint16_t*)&buffer[2]);
    *temp_ptr16 = GetDataAs16b(buffer[0], 0) ^ GetDataAs16b(buffer[1], 0);
    cycle_cnt++;

    for (int i = 0; i < 7; i++) {
        printf("Buffer %1d = 0x%16lx\n", i, buffer[i]);
    }
    cout << "Total cycles: " << cycle_cnt << endl;
    return buffer[2];
}

uint64_t ConvertFloat2Long(float af) {
    uint32_t *fptr = (uint32_t*)&af;
    uint64_t a = 0, amnt, aexp, asign;
    amnt = *fptr & 0x7FFFFFL;
    aexp = (*fptr >> 23) & 0xFFL;
    asign = *fptr >> 31;
    a = (1L << 63) | (amnt << (64-24)) | (aexp << 23) | (asign);
    return a;
}

float ConvertLong2Float(uint64_t al) {
    float *af = (float*)&al;
    uint64_t amnt, aexp, asign;
    amnt = (al & (0x7FFFFFL << 40)) >> 40;
    aexp = (al & (0xFFL << 23));
    asign = (al & 1) << 31;
    al = amnt | aexp | asign;
    return *af;
}

int main() {
    float af;
    float bf;
    float df;
    cin >> af >> bf;
    uint64_t a, b, d;
    a = ConvertFloat2Long(af);
    b = ConvertFloat2Long(bf);
    printf("0x%lx 0x%lx\n", a, b);
    cout << ConvertLong2Float(a) << " " << ConvertLong2Float(b) << " "
         << af*bf << endl;
    d = FloatMult(a, b);
    cout << ConvertLong2Float(d) << endl;
}