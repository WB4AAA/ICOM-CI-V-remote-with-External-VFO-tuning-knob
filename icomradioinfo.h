////////////////////////////////////////////////////////////////////////////////
// ICOM CI-V Parameters Read file
// Created by WB4AAA
////////////////////////////////////////////////////////////////////////////////


#ifndef ICOMREADINFO_H
#define ICOMREADINFO_H
#include <windows.h>
#include <vcl.h>
#include <string>
#include <stdio.h>

bool radio_id(HANDLE H, int a, char *b);                // Read Radio Model Number  1 byte  0-255
bool read_vfo(HANDLE H, int a, char *b);                // read current VFO frequency 5byte 0000000000-9999999999
bool write_vfo(HANDLE H, int a, char *b, int c, bool d);// Write current VFO frequency c-Step, d- up or down
bool rit_status(HANDLE H, int a, char *b);              // RIT status on/off. 1 byte 0 or 1
bool rit_on_off(HANDLE H, int a, bool b);               // RIT on/off switch 1 byte 0 or 1
bool rit_read_f(HANDLE H, int a, char *b);              // RIT frequency read 3 bytes. 0000-9999 and + or -
bool rit_write_f(HANDLE H, int a, char *b, bool d);     // RIT frequency write. 3 bytes 0000-9999 and + or -
bool read_mode_fil(HANDLE H, int a, char *b, char *c);  // Operation mode and filter width. 2 bytes. first byte mode. second byte filter
bool read_af(HANDLE H, int a, char *b);                 // Read audio frequency volume. 2 bytes 0-255
bool write_af(HANDLE H, int a, char *b, bool c);        // Write audio frequency volum. 2 bytes 0-255. c- up or down
bool read_rf(HANDLE H, int a, char *b);                 // Read radio frequency volume. 2 bytes 0-255
bool write_rf(HANDLE H, int a, char *b, bool c);        // Write radio frequency volum. 2 bytes 0-255. c- up or down
int read_vfo_err(HANDLE H, int a, char *b);
bool vfo_a_b(HANDLE H, int a, int b);                   // VFO A/B switch 1 byte. 0 or 1
bool signal_read(HANDLE H, int a, char *b);             // S meter level. 2 bytes 0-255

#endif
 