////////////////////////////////////////////////////////////////////////////////
// ICOM CI-V Parameters Read file
// Created by WB4AAA
////////////////////////////////////////////////////////////////////////////////

#include "icomreadinfo.h"
#include <stdio.h>
#include <stdint.h>
int bcd_to_int(unsigned char b1, unsigned char b2)
{
    return ((b1 >> 4) & 0x0F) * 1000 +
           (b1 & 0x0F) * 100  +
           ((b2 >> 4) & 0x0F) * 10 +
           (b2 & 0x0F);
}

uint64_t bcd5_to_int(const unsigned char bcd[5])
{
    uint64_t value = 0;

    for (int i = 0; i < 5; i++)
    {
        value = value * 10 + ((bcd[i] >> 4) & 0x0F);
        value = value * 10 + (bcd[i] & 0x0F);
    }

    return value;
}

void int_to_bcd(int value, unsigned int b1, unsigned int b2)
{
        int f=abs(value);
        int f1=f/100;
        f1=((f1/10)<<4)|(f1%10);
        b1=f1;
        int f2=f%100;
        f2=((f2/10)<<4)|(f2%10);
        b2=f2;
}

void int_to_bcd5(uint64_t value, unsigned char bcd[5])
{
    if (value > 9999999999ULL)
        value = 9999999999ULL;

    for (int i = 4; i >= 0; i--)
    {
        int d1 = value % 10;
        value /= 10;
        int d2 = value % 10;
        value /= 10;

        bcd[i] = ((d2 & 0x0F) << 4) | (d1 & 0x0F);
    }
}

bool radio_id(HANDLE H, int a, char *b)
{
        if (H==NULL) {return 0;}
        char radioID[7]={0xfe, 0xfe, 0, 0xe0, 0x19, 0, 0xfd};
        DWORD BytesRead, BytesWrite;
        char Buf[100];
        radioID[2]=a&0xff;
        WriteFile(H, radioID, 7, &BytesWrite, NULL);
        ReadFile(H, Buf, 100, &BytesRead, NULL);
        int r1=memcmp(radioID, Buf, 7);
        if (r1!=0) { return 0;}
        if (((Buf[BytesRead-1]&0xff) !=0xfd)||(BytesRead<7)) {return 0;}
        int ID=Buf[BytesRead-2]&0xff;
        sprintf(b, "%d", ID);
        return 1;
}
bool read_vfo(HANDLE H, int a, char *b)
{
        if (H==NULL) {return 0;}
        char Buf[100];
        DWORD BytesRead, BytesWrite;
        char read_f[6]={0xfe, 0xfe, 0, 0xe0, 0x03, 0xfd};
        read_f[2]=a&0xff;
        WriteFile(H, read_f, 6, &BytesWrite, NULL);
        ReadFile(H, Buf, 100, &BytesRead, NULL);
        int len=BytesRead;
        if (len<7) { return 0;}
        int r1=memcmp(Buf, read_f, 6);
        if (r1!=0) { return 0;}
        if ((Buf[len-1]&0xff) !=0xfd) {return 0;}
        char freq[5];
        for (int i=0; i<5; i++) {freq[i]=Buf[BytesRead-2-i];}

        unsigned int val=bcd5_to_int(freq);

        sprintf(b, "%02llu.%03llu.%03llu",
        val / 1000000,
        (val / 1000) % 1000,
        val % 1000);
        if (b[0]=='0') {b[0]=' ';}
        return 1;
}

int read_vfo_err(HANDLE H, int a, char *b)
{
        if (H==NULL) {return 0;}         // Port not available
        char Buf[100];
        DWORD BytesRead, BytesWrite;
        char read_f[6]={0xfe, 0xfe, 0, 0xe0, 0x03, 0xfd};
        read_f[2]=a&0xff;
        WriteFile(H, read_f, 6, &BytesWrite, NULL);
        ReadFile(H, Buf, 100, &BytesRead, NULL);
        int len=BytesRead;
        if (len<7) { return 1;}               //No responce from the radio
        int r1=memcmp(Buf, read_f, 6);
        if (r1!=0) { return 2;}               //Wrong Responce from the radio
        if ((Buf[len-1]&0xff) !=0xfd) {return 3;}  //Not a CI-V responce
        char freq[5];
        memcpy(freq, Buf+11, 5);
        int c[5];
        String Vfo;
        for (int i=0; i<5; i++)
        {
        c[i]=freq[4-i]&0xff;
        Vfo=Vfo+(IntToHex(c[i], 2));
        }
        int A=StrToInt(Vfo);
        Vfo=A;
        len=Vfo.Length();
        int k=9-len;
        Vfo=Vfo.SubString(1, 3-k)+"."+Vfo.SubString(4-k, 3)+"."+Vfo.SubString(7-k, 3);
        strcpy(b, Vfo.c_str());
        return 4;
}

bool signal_read(HANDLE H, int a, char *b)
{
        if (H==NULL) {return 0;}
        DWORD BytesRead, BytesWrite;
        char read_signal[7]={0xfe, 0xfe, 0, 0xe0, 0x15, 0x02, 0xfd};
        char Buf[100];
        read_signal[2]=a&0xff;
        WriteFile(H, read_signal, 7, &BytesWrite, NULL);
        ReadFile(H, Buf, 100, &BytesRead, NULL);
        int r1=memcmp(read_signal, Buf, 7);
        if ((r1!=0)||(BytesRead<7)||((Buf[BytesRead-1]&0xff)!=0xfd)) {return 0;}
        
        int value = bcd_to_int(Buf[BytesRead-3], Buf[BytesRead-2]);

        sprintf(b, "%d", value);

        return 1;
}

bool write_vfo(HANDLE H, int a, char *b, int c, bool d)
{
        if (H==NULL) {return 0;}
        char read_f[6]={0xfe, 0xfe, 0, 0xe0, 0x03, 0xfd};
        char write_f[11]={0xfe, 0xfe, 0, 0xe0, 0x05, 0, 0, 0, 0, 0, 0xfd};
        DWORD BytesRead, BytesWrite;
        char buffer[100];
        int Step=c;
        read_f[2]=a&0xff;
        write_f[2]=a&0xff;
        WriteFile(H, read_f, 6, &BytesWrite, NULL);
        ReadFile(H, buffer, 100, &BytesRead, NULL);
        int r1=memcmp(buffer, read_f, 6);
        if ((r1!=0)||(BytesRead<7)||((buffer[BytesRead-1]&0xff)!=0xfd)) {return 0;}
        char freq[5];
        for (int i=0; i<5; i++) {freq[i]=buffer[BytesRead-2-i];}
        int val=bcd5_to_int(freq);

        if (d==false)
        {
        val=val-Step;
        }
        if (d==true)
        {
        val=val+Step;
        }

        int_to_bcd5(val, freq);
        for (int i=0; i<5; i++) {write_f[9-i]=freq[i];}


        WriteFile(H, write_f, 11, &BytesWrite, NULL);
        ReadFile(H, buffer, 100, &BytesWrite, NULL);
        r1=memcmp(write_f, buffer, 11);
        if ((r1!=0)||((buffer[BytesRead-1]&0xff)!=0xfd)||((buffer[BytesRead-2]&0xff)!=0xfb)) { return 0;}

        sprintf(b, "%02llu.%03llu.%03llu",
        val / 1000000,
        (val / 1000) % 1000,
        val % 1000);
        if (b[0]=='0') {b[0]=' ';}

        return 1;
}

bool rit_status(HANDLE H, int a, char *b)
{
       
        if (H==NULL) {return 0 ;}
        char status[7]={0xfe, 0xfe, 0x0, 0xe0, 0x21, 0x01, 0xfd};
        DWORD BytesRead, BytesWrite;
        char Buf[100];
        status[2]=a&0xff;
        WriteFile(H, status, 7, &BytesWrite, NULL);
        ReadFile(H, Buf, 100, &BytesRead, NULL);
        int r1=memcmp(status, Buf, 7);
        if ((r1!=0)||(BytesRead<7)||((Buf[BytesRead-1]&0xff)!=0xfd)) {return 0;}
        int c=Buf[BytesRead-2]&0xff;
        sprintf(b, "%d", c);
        return 1;
}

bool rit_on_off(HANDLE H, int a, bool b)
{
        if (H==NULL) {return 0;}
        char ritSwitch[8]={0xfe, 0xfe, 0, 0xe0, 0x21, 0x01, 0, 0xfd};
        DWORD BytesRead, BytesWrite;
        char Buf[100];
        ritSwitch[2]=a&0xff;
        if (b==true) {ritSwitch[6]=1&0xff;}
        if (b==false) {ritSwitch[6]=0&0xff;}
        WriteFile(H, ritSwitch, 8, &BytesWrite, NULL);
        ReadFile(H, Buf, 100, &BytesRead, NULL);
        int r1 = memcmp(ritSwitch, Buf, 8);
        if ((r1!=0)||(BytesRead<8)||((Buf[BytesRead-1]&0xff)!=0xfd)) {return 0;}
        if ((Buf[BytesRead-2]&0xff) == 0xfa) {return 0;}
        return 1;
}

bool rit_read_f(HANDLE H, int a, char *b)
{
        if (H==NULL) {return 0;}
        char ritRead[7]={0xfe, 0xfe, 0, 0xe0, 0x21, 0, 0xfd};
        DWORD BytesRead, BytesWrite;
        char Buf[100];
        ritRead[2]=a&0xff;
        WriteFile(H, ritRead, 7, &BytesWrite, NULL);
        ReadFile(H, Buf, 100, &BytesRead, NULL);
        int r1=memcmp(ritRead, Buf, 7);
        if ((r1!=0)||(BytesRead<7)||((Buf[BytesRead-1]&0xff)!=0xfd)) {return 0;}

        int sign = Buf[BytesRead-2]&0xff;
        int hi = Buf[BytesRead-3]&0xff;
        int lo = Buf[BytesRead-4]&0xff;
        int value = (hi << 8) | lo;

        char hex[5];
        sprintf(hex, "%04X", value);

        char formatted[8];
        sprintf(formatted, "%c.%c%c", hex[0], hex[1], hex[2]);

        if (sign == 0)
        sprintf(b, " %s", formatted);
        else
        sprintf(b, "-%s", formatted);

        return 1;
}

bool rit_write_f(HANDLE H, int a, char *b, bool d)
{
        if (H==NULL) {return 0;}
        char ritRead[7]={0xfe, 0xfe, 0, 0xe0, 0x21, 0, 0xfd};
        char ritWrite[10]={0xfe, 0xfe, 0, 0xe0, 0x21, 0, 0, 0, 0, 0xfd};
        DWORD BytesRead, BytesWrite;
        char Buf[100];
        ritRead[2]=a&0xff;
        ritWrite[2]=a&0xff;
        WriteFile(H, ritRead, 7, &BytesWrite, NULL);
        ReadFile(H, Buf, 100, &BytesRead, NULL);
        int r1=memcmp(ritRead, Buf, 7);
        if ((r1!=0)||(BytesRead<7)||((Buf[BytesRead-1]&0xff)!=0xfd)) {return 0;}

        int sign = Buf[BytesRead - 2] & 0xFF;     // 0=+, 1=-
        unsigned char b1 = Buf[BytesRead - 3];
        unsigned char b2 = Buf[BytesRead - 4];

        int value = bcd_to_int(b1, b2);
        if (sign==1) value = -value;

        if (d==true) {value = value + 10;}
        if (d==false) {value = value - 10;}
        unsigned int v1, v2;
        int f=abs(value);
        int f1=f/100;
        f1=((f1/10)<<4)|(f1%10);
        int f2=f%100;
        f2=((f2/10)<<4)|(f2%10);
        if (value<0) {ritWrite[8]=1&0xff;}
        if (value>=0) {ritWrite[8]=0&0xff;}
        ritWrite[7]=f1&0xff;
        ritWrite[6]=f2&0xff;

        WriteFile(H, ritWrite, 10, &BytesWrite, NULL);
        ReadFile(H, Buf, 100, &BytesRead, NULL);
        r1=memcmp(ritWrite, Buf, 10);
        if ((r1!=0)||(BytesRead<7)||((Buf[BytesRead-1]&0xff)!=0xfd)) {return 0;}
        if ((Buf[BytesRead-2]&0xff)==0xfa) {return 0;}


        int absval = abs(value/10);
        sprintf(b, "%c%d.%02d",
            value < 0 ? '-' : ' ',
            absval / 100,
            absval % 100);
        return 1;
}

bool read_af(HANDLE H, int a, char *b)
{
        if (H==NULL) {return 0;}
        char readAf[7]={0xfe, 0xfe, 0, 0xe0, 0x14, 1, 0xfd};
        DWORD BytesRead, BytesWrite;
        char Buf[100];
        readAf[2]=a&0xff;
        WriteFile(H, readAf, 7, &BytesWrite, NULL);
        ReadFile(H, Buf, 100, &BytesRead, NULL);
        int r1=memcmp(readAf, Buf, 7);
        if ((r1!=0)||(BytesRead<7)||((Buf[BytesRead-1]&0xff)!=0xfd)) {return 0;}
        int value = bcd_to_int(Buf[BytesRead-3], Buf[BytesRead-2]);

        sprintf(b, "%d", value);

        return 1;

}

bool write_af(HANDLE H, int a, char *b, bool c)
{
        if (H==NULL) {return 0;}
        char readAf[7]={0xfe, 0xfe, 0, 0xe0, 0x14, 1, 0xfd};
        char writeAf[9]={0xfe, 0xfe, 0, 0xe0, 0x14, 1, 0, 0, 0xfd};
        DWORD BytesRead, BytesWrite;
        char Buf[100];
        readAf[2]=a&0xff;
        writeAf[2]=a&0xff;
        WriteFile(H, readAf, 7, &BytesWrite, NULL);
        ReadFile(H, Buf, 100, &BytesRead, NULL);
        int r1=memcmp(readAf, Buf, 7);
        if ((r1!=0)||(BytesRead<7)||((Buf[BytesRead-1]&0xff)!=0xfd)) {return 0;}
        unsigned int value = bcd_to_int(Buf[BytesRead-3], Buf[BytesRead-2]);
        if (c==true) {value=value+1;}
        if (c==false) {value=value-1;}
        unsigned int lo, hi;
        hi=value/100;
        lo=value%100;
        hi=((hi/10)<<4)|(hi%10);
        lo=((lo/10)<<4)|(lo&10);
        writeAf[6]=hi&0xff;
        writeAf[7]=lo&0xff;
        WriteFile(H, writeAf, 9, &BytesWrite, NULL);
        ReadFile(H, Buf, 100, &BytesRead, NULL);
        r1=memcmp(writeAf, Buf, 9);
        if ((r1!=0)||(BytesRead<7)||((Buf[BytesRead-1]&0xff)!=0xfd)) {return 0;}
        if ((Buf[BytesRead-2]&0xff)==0xfa) {return 0;}
        sprintf(b, "&%d", value);
}

bool read_rf(HANDLE H, int a, char *b)
{
        if (H==NULL) {return 0;}
        char readRf[7]={0xfe, 0xfe, 0, 0xe0, 0x14, 2, 0xfd};
        DWORD BytesRead, BytesWrite;
        char Buf[100];
        readRf[2]=a&0xff;
        WriteFile(H, readRf, 7, &BytesWrite, NULL);
        ReadFile(H, Buf, 100, &BytesRead, NULL);
        int r1=memcmp(readRf, Buf, 7);
        if ((r1!=0)||(BytesRead<7)||((Buf[BytesRead-1]&0xff)!=0xfd)) {return 0;}
        int value = bcd_to_int(Buf[BytesRead-3], Buf[BytesRead-2]);

        sprintf(b, "%d", value);

        return 1;

}

bool write_rf(HANDLE H, int a, char *b, bool c)
{
        if (H==NULL) {return 0;}
        char readRf[7]={0xfe, 0xfe, 0, 0xe0, 0x14, 2, 0xfd};
        char writeRf[9]={0xfe, 0xfe, 0, 0xe0, 0x14, 2, 0, 0, 0xfd};
        DWORD BytesRead, BytesWrite;
        char Buf[100];
        readRf[2]=a&0xff;
        writeRf[2]=a&0xff;
        WriteFile(H, readRf, 7, &BytesWrite, NULL);
        ReadFile(H, Buf, 100, &BytesRead, NULL);
        int r1=memcmp(readRf, Buf, 7);
        if ((r1!=0)||(BytesRead<7)||((Buf[BytesRead-1]&0xff)!=0xfd)) {return 0;}
        unsigned int value = bcd_to_int(Buf[BytesRead-3], Buf[BytesRead-2]);
        if (c==true) {value=value+1;}
        if (c==false) {value=value-1;}
        unsigned int lo, hi;
        hi=value/100;
        lo=value%100;
        hi=((hi/10)<<4)|(hi%10);
        lo=((lo/10)<<4)|(lo&10);
        writeRf[6]=hi&0xff;
        writeRf[7]=lo&0xff;
        WriteFile(H, writeRf, 9, &BytesWrite, NULL);
        ReadFile(H, Buf, 100, &BytesRead, NULL);
        r1=memcmp(writeRf, Buf, 7);
        if ((r1!=0)||(BytesRead<7)||((Buf[BytesRead-1]&0xff)!=0xfd)) {return 0;}
        if ((Buf[BytesRead-2]&0xff)==0xfa) {return 0;}
        sprintf(b, "&%d", value);
}

bool vfo_a_b(HANDLE H, int a, int b)
{
        char vfoAB[7]={0xfe, 0xfe, 0, 0xe0, 7, 0, 0xfd};
        DWORD BytesRead, BytesWrite;
        char Buf[100];
        vfoAB[2]=a&0xff;
        vfoAB[5]=b&0xff;
        WriteFile(H, vfoAB, 7, &BytesWrite, NULL);
        ReadFile(H, Buf, 100, &BytesRead, NULL);
        int r1=memcmp(vfoAB, Buf, 7);
        if ((r1!=0)||(BytesRead<7)||((Buf[BytesRead-1]&0xff)!=0xfd)) {return 0;}
        if ((Buf[BytesRead-1]&0xff)==0xfa) {return 0;}

        return 1;
}

bool read_mode_fil(HANDLE H, int a, char *b, char *c)
{
        if (H==NULL) {return 0;}
        char readMode[6]={0xfe, 0xfe, 0, 0xe0, 4, 0xfd};
        DWORD BytesRead, BytesWrite;
        char Buf[100];
        readMode[2]=a&0xff;
        WriteFile(H, readMode, 6, &BytesWrite, NULL);
        ReadFile(H, Buf, 100, &BytesRead, NULL);
        int r1=memcmp(readMode, Buf, 6);
        if ((r1!=0)||(BytesRead<7)||((Buf[BytesRead-1]&0xff)!=0xfd)) {return 0;}
        int fl=Buf[BytesRead-2]&0xff;
        int md=Buf[BytesRead-3]&0xff;
        sprintf(b, "%d", md);
        sprintf(c, "%d", fl);
        return 1;
}
