#include "pwm.h"
#include "hw.h"

extern char _binary_tune_wav_start;

static volatile unsigned* gpio = (void*)GPIO_BASE;
static volatile unsigned* clk = (void*)CLOCK_BASE;
static volatile unsigned* pwm = (void*)PWM_BASE;

//const char audio_data[5399]={0x39, 0x26, 0x26, 0x26, 0x26, 0x51, 0x77, 0x8C, 0x7C, 0x85, 0xB2, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xB6, 0x85, 0x6A, 0x5A, 0x39, 0x30, 0x2C, 0x51, 0x61, 0x56, 0x51, 0x61, 0x7E, 0x93, 0xB8, 0xD7, 0xDA, 0xCE, 0xA2, 0x73, 0x46, 0x3B, 0x32, 0x39, 0x50, 0x66, 0x6A, 0x5D, 0x5D, 0x5E, 0x85, 0xAF, 0xDA, 0xDA, 0xDA, 0xBF, 0x85, 0x46, 0x26, 0x26, 0x26, 0x26, 0x39, 0x5A, 0x74, 0x87, 0x7C, 0x74, 0x74, 0x7F, 0x7F, 0x7F, 0xA6, 0xC8, 0xDA, 0xBF, 0x9B, 0x68, 0x56, 0x4D, 0x6F, 0xA0, 0xDA, 0xDA, 0xDA, 0xDA, 0xC9, 0x7F, 0x5A, 0x37, 0x37, 0x5E, 0x7F, 0x9B, 0x82, 0x5E, 0x32, 0x2C, 0x26, 0x39, 0x68, 0xB2, 0xDA, 0xDA, 0xDA, 0xCC, 0xA6, 0x79, 0x61, 0x66, 0x79, 0x94, 0x94, 0x8C, 0x73, 0x50, 0x30, 0x26, 0x29, 0x48, 0x8E, 0xC8, 0xDA, 0xDA, 0xBD, 0x90, 0x66, 0x50, 0x54, 0x5D, 0x68, 0x6C, 0x73, 0x6A, 0x70, 0x6A, 0x6C, 0x66, 0x79, 0x87, 0x89, 0x7F, 0x73, 0x58, 0x3B, 0x26, 0x32, 0x4B, 0x6C, 0x7E, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x90, 0xA0, 0xA6, 0x9D, 0x7C, 0x74, 0x9B, 0xDA, 0xDA, 0xDA, 0xDA, 0xC1, 0x8C, 0x7F, 0x7C, 0x51, 0x39, 0x2C, 0x42, 0x54, 0x48, 0x50, 0x3B, 0x2D, 0x29, 0x50, 0x90, 0xCE, 0xDA, 0xDA, 0xDA, 0xDA, 0xB6, 0x8C, 0x7C, 0x6A, 0x5A, 0x54, 0x66, 0x6C, 0x58, 0x3E, 0x29, 0x26, 0x26, 0x39, 0x77, 0xBA, 0xDA, 0xDA, 0xDA, 0xC8, 0x9D, 0x70, 0x5E, 0x58, 0x7C, 0x7F, 0x85, 0x79, 0x73, 0x5A, 0x39, 0x26, 0x2D, 0x42, 0x61, 0x89, 0xA2, 0x89, 0x6A, 0x5D, 0x5E, 0x54, 0x50, 0x61, 0x6C, 0x70, 0x74, 0x7E, 0x7F, 0x7F, 0x85, 0x82, 0xA2, 0xAD, 0xBF, 0x90, 0x7F, 0x6F, 0x5A, 0x50, 0x6C, 0xD3, 0xDA, 0xDA, 0xDA, 0xDA, 0xB6, 0x7F, 0x7F, 0x6C, 0x61, 0x5A, 0x51, 0x48, 0x4D, 0x5D, 0x46, 0x26, 0x26, 0x26, 0x5A, 0x8C, 0xD0, 0xDA, 0xDA, 0xDA, 0xDA, 0xA6, 0x93, 0x73, 0x5A, 0x5A, 0x7C, 0x94, 0x8C, 0x8C, 0x8C, 0x73, 0x5D, 0x5E, 0x87, 0x97, 0x8C, 0x74, 0x5D, 0x4D, 0x2C, 0x2C, 0x3F, 0x6A, 0x82, 0x93, 0xB2, 0xBD, 0xA0, 0x73, 0x56, 0x50, 0x42, 0x42, 0x4B, 0x6F, 0x85, 0x7F, 0x79, 0x68, 0x6A, 0x61, 0x5D, 0x61, 0x6A, 0x70, 0x74, 0x74, 0x7C, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x85, 0xB2, 0xDA, 0xDA, 0xDA, 0xDA, 0xB6, 0x7F, 0x7F, 0x7F, 0x7F, 0x6A, 0x39, 0x26, 0x26, 0x26, 0x26, 0x28, 0x26, 0x26, 0x3F, 0x6C, 0x99, 0xC8, 0xDA, 0xDA, 0xDA, 0xDA, 0xC1, 0x8C, 0x7F, 0x7F, 0x74, 0x68, 0x6A, 0x73, 0x7F, 0x66, 0x46, 0x26, 0x26, 0x26, 0x26, 0x5A, 0x8E, 0xC5, 0xDA, 0xDA, 0xDA, 0xD3, 0x9B, 0x73, 0x58, 0x50, 0x56, 0x5A, 0x68, 0x63, 0x4D, 0x37, 0x29, 0x2C, 0x39, 0x46, 0x61, 0x7C, 0x9B, 0xA6, 0xA2, 0x90, 0x7C, 0x73, 0x6A, 0x6A, 0x74, 0x7C, 0x85, 0x82, 0x6F, 0x5A, 0x54, 0x6C, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x85, 0xC8, 0xDA, 0xDA, 0xDA, 0xDA, 0xA6, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x5A, 0x3E, 0x26, 0x26, 0x26, 0x26, 0x26, 0x26, 0x4B, 0x74, 0x97, 0xCC, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xB6, 0x7F, 0x7F, 0x7F, 0x7E, 0x66, 0x61, 0x68, 0x6F, 0x58, 0x3E, 0x39, 0x37, 0x30, 0x3E, 0x63, 0x8C, 0xA4, 0xB8, 0xCC, 0xCC, 0xB3, 0x94, 0x87, 0x89, 0x87, 0x85, 0x77, 0x74, 0x5E, 0x46, 0x26, 0x26, 0x26, 0x26, 0x35, 0x56, 0x85, 0x97, 0xA2, 0xA6, 0x9D, 0x8C, 0x77, 0x74, 0x6F, 0x79, 0x7F, 0x7F, 0x7C, 0x68, 0x68, 0x5A, 0x6C, 0x73, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x85, 0xC8, 0xDA, 0xDA, 0xDA, 0xDA, 0xB6, 0x7F, 0x7F, 0x7F, 0x7F, 0x7C, 0x56, 0x3E, 0x2C, 0x37, 0x29, 0x26, 0x26, 0x26, 0x29, 0x4B, 0x6C, 0xAD, 0xD3, 0xDA, 0xDA, 0xDA, 0xDA, 0xCC, 0xA0, 0x8C, 0x7C, 0x73, 0x73, 0x85, 0x8E, 0x89, 0x6C, 0x5A, 0x2C, 0x26, 0x26, 0x26, 0x42, 0x6C, 0x93, 0xAF, 0xC4, 0xC5, 0xBA, 0xA6, 0x9B, 0x89, 0x7E, 0x85, 0x90, 0x99, 0x7F, 0x74, 0x56, 0x3F, 0x26, 0x26, 0x26, 0x3E, 0x5E, 0x77, 0x8C, 0x93, 0x90, 0x87, 0x7E, 0x79, 0x73, 0x70, 0x7C, 0x93, 0x93, 0x79, 0x66, 0x58, 0x56, 0x5A, 0x6C, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x97, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xA0, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x66, 0x50, 0x26, 0x26, 0x26, 0x26, 0x26, 0x26, 0x3F, 0x48, 0x6C, 0x9B, 0xC8, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xC1, 0xA2, 0x8C, 0x7E, 0x6A, 0x54, 0x58, 0x70, 0x7C, 0x5E, 0x56, 0x3E, 0x29, 0x26, 0x26, 0x3E, 0x58, 0x87, 0xA9, 0xCE, 0xDA, 0xDA, 0xC5, 0xB2, 0xA0, 0x8E, 0x77, 0x7F, 0x79, 0x6F, 0x5D, 0x56, 0x39, 0x2D, 0x26, 0x3B, 0x3F, 0x63, 0x6F, 0x8E, 0x97, 0xA9, 0x94, 0x94, 0x89, 0x7F, 0x70, 0x70, 0x74, 0x77, 0x77, 0x79, 0x6F, 0x68, 0x5D, 0x5A, 0x61, 0x73, 0x7F, 0x7F, 0x7F, 0x85, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xB6, 0x7F, 0x7F, 0x7F, 0x7F, 0x70, 0x56, 0x50, 0x54, 0x5E, 0x44, 0x26, 0x26, 0x26, 0x26, 0x26, 0x4B, 0x7E, 0x9B, 0xC8, 0xDA, 0xDA, 0xDA, 0xDA, 0xD7, 0xB2, 0xA2, 0x97, 0x90, 0x85, 0x7F, 0x7F, 0x7C, 0x66, 0x44, 0x26, 0x26, 0x26, 0x26, 0x26, 0x4D, 0x74, 0x97, 0xAD, 0xBA, 0xC1, 0xB6, 0xC5, 0xBA, 0xC1, 0xAF, 0xA4, 0x97, 0x85, 0x74, 0x63, 0x48, 0x3B, 0x2C, 0x30, 0x37, 0x48, 0x58, 0x6F, 0x77, 0x87, 0x8E, 0x8E, 0x8E, 0x85, 0x87, 0x7E, 0x7C, 0x7C, 0x82, 0x94, 0x94, 0x90, 0x73, 0x68, 0x4D, 0x48, 0x48, 0x61, 0x74, 0x9B, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xB6, 0x7F, 0x7F, 0x7F, 0x7C, 0x5A, 0x44, 0x2C, 0x39, 0x50, 0x48, 0x46, 0x29, 0x2C, 0x26, 0x29, 0x4B, 0x7F, 0x7F, 0x7F, 0xB2, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xC4, 0xAF, 0x9D, 0x90, 0x8C, 0x85, 0x85, 0x7F, 0x73, 0x5A, 0x30, 0x26, 0x26, 0x26, 0x26, 0x4B, 0x63, 0x87, 0xA4, 0xC1, 0xB6, 0xC9, 0xBD, 0xC8, 0xA6, 0xA6, 0x94, 0x97, 0x82, 0x77, 0x6A, 0x5D, 0x54, 0x46, 0x3F, 0x37, 0x3E, 0x44, 0x56, 0x5E, 0x79, 0x89, 0x97, 0x99, 0x97, 0x8E, 0x93, 0x8C, 0x89, 0x7E, 0x7F, 0x82, 0x85, 0x7F, 0x73, 0x5D, 0x56, 0x48, 0x51, 0x5D, 0x87, 0xBF, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xB6, 0x8C, 0x7F, 0x6C, 0x50, 0x39, 0x37, 0x44, 0x4D, 0x4D, 0x48, 0x42, 0x37, 0x29, 0x26, 0x3F, 0x61, 0x7C, 0x7F, 0x85, 0xAF, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xC5, 0xB6, 0xAB, 0xA0, 0x97, 0x90, 0x8C, 0x7F, 0x74, 0x5E, 0x44, 0x28, 0x26, 0x26, 0x2C, 0x48, 0x66, 0x7F, 0xA2, 0xAB, 0xB6, 0xB2, 0xB2, 0xA0, 0x93, 0x89, 0x7E, 0x7F, 0x77, 0x79, 0x7F, 0x89, 0x7E, 0x70, 0x61, 0x50, 0x46, 0x46, 0x56, 0x5E, 0x6F, 0x87, 0x97, 0xA4, 0xA2, 0xA4, 0x99, 0x93, 0x82, 0x7E, 0x73, 0x6F, 0x6A, 0x68, 0x61, 0x51, 0x51, 0x3E, 0x4D, 0x50, 0x77, 0xA2, 0xC4, 0xDA, 0xDA, 0xDA, 0xDA, 0xC9, 0xA4, 0x85, 0x5D, 0x46, 0x3B, 0x37, 0x42, 0x56, 0x63, 0x6F, 0x6A, 0x5E, 0x51, 0x3F, 0x3E, 0x4B, 0x68, 0x7F, 0x85, 0xA6, 0xC8, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xD7, 0xCC, 0xA2, 0x8C, 0x7F, 0x7F, 0x7F, 0x7E, 0x68, 0x50, 0x30, 0x26, 0x2D, 0x30, 0x46, 0x50, 0x74, 0x87, 0x93, 0x90, 0x94, 0x93, 0x9D, 0x9B, 0xA2, 0x94, 0x97, 0x8E, 0x9D, 0x97, 0xA0, 0x89, 0x7C, 0x66, 0x5A, 0x58, 0x58, 0x5D, 0x63, 0x70, 0x7C, 0x90, 0x94, 0x94, 0x8C, 0x7C, 0x68, 0x56, 0x50, 0x61, 0x56, 0x63, 0x61, 0x77, 0x6F, 0x5D, 0x48, 0x4B, 0x61, 0x74, 0x9B, 0xB2, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xA6, 0x7F, 0x5D, 0x48, 0x50, 0x48, 0x61, 0x77, 0x8E, 0x93, 0x74, 0x5E, 0x4B, 0x42, 0x3F, 0x30, 0x3F, 0x50, 0x77, 0x97, 0xAD, 0xCE, 0xD7, 0xD7, 0xCC, 0xBF, 0xC1, 0xAB, 0xAD, 0xA6, 0xAD, 0xA0, 0xA2, 0x93, 0x89, 0x70, 0x5D, 0x44, 0x3E, 0x35, 0x3B, 0x3E, 0x58, 0x6A, 0x79, 0x8E, 0x94, 0x93, 0x94, 0x97, 0x94, 0x8C, 0x7F, 0x8E, 0x8C, 0x9B, 0x9B, 0x9B, 0x97, 0x82, 0x74, 0x68, 0x63, 0x5D, 0x51, 0x54, 0x61, 0x6C, 0x7C, 0x6C, 0x70, 0x63, 0x61, 0x5D, 0x61, 0x73, 0x74, 0x74, 0x6C, 0x61, 0x5D, 0x5A, 0x6A, 0x74, 0x7F, 0xAB, 0xC4, 0xDA, 0xDA, 0xDA, 0xD5, 0xA2, 0x7C, 0x6C, 0x68, 0x61, 0x66, 0x77, 0x8E, 0x94, 0x89, 0x82, 0x77, 0x68, 0x56, 0x48, 0x44, 0x54, 0x5E, 0x5E, 0x7C, 0x8E, 0xA2, 0xA2, 0xA0, 0x99, 0x94, 0x93, 0x90, 0x9D, 0xAD, 0xC1, 0xC1, 0xCC, 0xC5, 0xB6, 0x99, 0x7E, 0x66, 0x50, 0x3B, 0x37, 0x3E, 0x56, 0x68, 0x77, 0x7C, 0x7C, 0x7F, 0x7C, 0x63, 0x5D, 0x50, 0x66, 0x68, 0x89, 0x93, 0xA6, 0xB2, 0xBA, 0xAF, 0x9B, 0x89, 0x7E, 0x74, 0x6C, 0x6F, 0x73, 0x7C, 0x7C, 0x82, 0x82, 0x87, 0x85, 0x7E, 0x66, 0x5E, 0x56, 0x63, 0x5E, 0x70, 0x77, 0x7E, 0x6F, 0x5E, 0x54, 0x4D, 0x48, 0x4B, 0x61, 0x74, 0x94, 0xB2, 0xDA, 0xDA, 0xDA, 0xDA, 0xBD, 0x99, 0x85, 0x74, 0x70, 0x70, 0x90, 0x9B, 0xAD, 0xA6, 0xA4, 0x90, 0x77, 0x56, 0x3E, 0x30, 0x35, 0x3E, 0x54, 0x68, 0x7F, 0x90, 0x90, 0x8C, 0x7F, 0x7F, 0x7E, 0x7F, 0x87, 0x90, 0xB2, 0xBA, 0xC8, 0xC5, 0xB8, 0xAD, 0x8C, 0x7E, 0x63, 0x54, 0x5A, 0x61, 0x6A, 0x6F, 0x6F, 0x73, 0x6C, 0x5E, 0x50, 0x4B, 0x56, 0x63, 0x6A, 0x82, 0x9D, 0xB2, 0xC1, 0xBF, 0xB2, 0x9B, 0x89, 0x77, 0x6A, 0x63, 0x70, 0x77, 0x87, 0x8C, 0x90, 0x87, 0x79, 0x70, 0x5E, 0x63, 0x63, 0x77, 0x7E, 0x85, 0x89, 0x8E, 0x8E, 0x82, 0x7E, 0x6C, 0x68, 0x5D, 0x5A, 0x5E, 0x66, 0x73, 0x70, 0x74, 0x7F, 0x87, 0x87, 0x8E, 0x97, 0x9B, 0x9D, 0x97, 0x9D, 0x9B, 0xB2, 0xAB, 0xAF, 0xA2, 0x9D, 0x93, 0x89, 0x89, 0x87, 0x7E, 0x77, 0x68, 0x6C, 0x6A, 0x70, 0x70, 0x6F, 0x70, 0x66, 0x6A, 0x6A, 0x70, 0x7F, 0x7C, 0x8E, 0x85, 0xA4, 0x9B, 0xA6, 0x8E, 0x8C, 0x89, 0x87, 0x89, 0x7F, 0x82, 0x7E, 0x74, 0x6F, 0x6A, 0x5D, 0x66, 0x6C, 0x79, 0x70, 0x6F, 0x74, 0x7F, 0x85, 0x87, 0x8C, 0x8C, 0x87, 0x7C, 0x7F, 0x85, 0x94, 0x90, 0x93, 0x82, 0x82, 0x7C, 0x7F, 0x70, 0x7F, 0x73, 0x7E, 0x74, 0x7C, 0x79, 0x7F, 0x77, 0x7F, 0x73, 0x85, 0x7C, 0x85, 0x7F, 0x85, 0x82, 0x7C, 0x82, 0x7E, 0x85, 0x7E, 0x79, 0x77, 0x79, 0x7F, 0x7C, 0x73, 0x73, 0x70, 0x77, 0x7F, 0x7F, 0x8C, 0x82, 0x90, 0x87, 0x93, 0x90, 0x8E, 0x87, 0x85, 0x97, 0x93, 0x9B, 0x8C, 0x94, 0x82, 0x87, 0x7F, 0x82, 0x77, 0x70, 0x77, 0x7E, 0x7C, 0x7F, 0x7E, 0x7C, 0x79, 0x7F, 0x85, 0x82, 0x85, 0x87, 0x89, 0x7F, 0x82, 0x7F, 0x7C, 0x77, 0x74, 0x6A, 0x7E, 0x7E, 0x7F, 0x70, 0x74, 0x7E, 0x7E, 0x7C, 0x7E, 0x7E, 0x77, 0x7E, 0x7E, 0x8C, 0x82, 0x87, 0x7E, 0x85, 0x7E, 0x7F, 0x7E, 0x7F, 0x85, 0x82, 0x7E, 0x7E, 0x82, 0x82, 0x79, 0x77, 0x77, 0x85, 0x7F, 0x7F, 0x7C, 0x7F, 0x85, 0x7F, 0x7C, 0x79, 0x82, 0x7F, 0x82, 0x7F, 0x89, 0x7F, 0x7E, 0x7F, 0x89, 0x82, 0x7F, 0x7C, 0x82, 0x82, 0x87, 0x85, 0x7F, 0x7F, 0x7F, 0x7E, 0x7F, 0x7F, 0x82, 0x7F, 0x8C, 0x7C, 0x79, 0x70, 0x7F, 0x87, 0x85, 0x82, 0x79, 0x79, 0x7F, 0x82, 0x82, 0x82, 0x7F, 0x7F, 0x7E, 0x85, 0x90, 0x85, 0x7F, 0x74, 0x7F, 0x8E, 0x8C, 0x7E, 0x85, 0x73, 0x8E, 0x7C, 0x7F, 0x74, 0x7F, 0x8E, 0x8E, 0x7F, 0x73, 0x70, 0x74, 0x8E, 0x7F, 0x8C, 0x7F, 0x7F, 0x82, 0x7F, 0x82, 0x7E, 0x74, 0x7F, 0x74, 0x7F, 0x7F, 0x7C, 0x7E, 0x7E, 0x82, 0x7F, 0x7F, 0x7F, 0x7E, 0x7C, 0x7C, 0x7C, 0x7F, 0x79, 0x85, 0x93, 0x87, 0x7C, 0x73, 0x85, 0x87, 0x7F, 0x8E, 0x82, 0x82, 0x73, 0x7F, 0x7E, 0x7F, 0x7F, 0x7F, 0x7E, 0x7E, 0x82, 0x82, 0x7E, 0x7C, 0x82, 0x82, 0x82, 0x7F, 0x82, 0x7E, 0x7E, 0x7E, 0x85, 0x7F, 0x7F, 0x7F, 0x82, 0x7F, 0x7E, 0x7F, 0x8E, 0x7F, 0x79, 0x73, 0x82, 0x87, 0x7F, 0x7E, 0x7E, 0x8E, 0x7F, 0x7F, 0x73, 0x7F, 0x8E, 0x7F, 0x8E, 0x73, 0x7E, 0x73, 0x7F, 0x82, 0x7F, 0x8E, 0x82, 0x7F, 0x70, 0x7C, 0x82, 0x85, 0x7E, 0x7E, 0x8E, 0x93, 0x8E, 0x7F, 0x7F, 0x74, 0x7E, 0x73, 0x8C, 0x73, 0x7F, 0x74, 0x7E, 0x7F, 0x7F, 0x82, 0x7F, 0x7F, 0x7E, 0x7F, 0x7E, 0x85, 0x7F, 0x7F, 0x7E, 0x7E, 0x7F, 0x82, 0x82, 0x7F, 0x8C, 0x7E, 0x82, 0x74, 0x82, 0x7E, 0x7E, 0x7F, 0x7E, 0x7F, 0x7E, 0x7F, 0x82, 0x85, 0x7E, 0x7E, 0x7F, 0x82, 0x7E, 0x7F, 0x7E, 0x82, 0x7F, 0x85, 0x7E, 0x7C, 0x82, 0x7F, 0x85, 0x7C, 0x82, 0x7C, 0x85, 0x7C, 0x82, 0x7F, 0x87, 0x7E, 0x7E, 0x7C, 0x7F, 0x82, 0x7F, 0x82, 0x7C, 0x85, 0x7E, 0x7F, 0x77, 0x7F, 0x7F, 0x85, 0x82, 0x82, 0x82, 0x7C, 0x7F, 0x82, 0x82, 0x7F, 0x7F, 0x82, 0x7F, 0x7F, 0x82, 0x82, 0x8C, 0x8C, 0x7E, 0x73, 0x74, 0x7F, 0x7E, 0x7F, 0x7F, 0x85, 0x7F, 0x7F, 0x7C, 0x7E, 0x7C, 0x7E, 0x7F, 0x7F, 0x85, 0x7E, 0x87, 0x7F, 0x82, 0x7F, 0x7E, 0x7F, 0x7F, 0x85, 0x7F, 0x8C, 0x7F, 0x82, 0x73, 0x7E, 0x7E, 0x7F, 0x7F, 0x7E, 0x7F, 0x7C, 0x7F, 0x82, 0x87, 0x7E, 0x7E, 0x7F, 0x82, 0x79, 0x7F, 0x7E, 0x85, 0x7F, 0x82, 0x7F, 0x7C, 0x85, 0x7E, 0x85, 0x7C, 0x85, 0x7C, 0x85, 0x7E, 0x82, 0x7F, 0x82, 0x7F, 0x7E, 0x82, 0x7C, 0x82, 0x79, 0x85, 0x7C, 0x82, 0x7C, 0x7F, 0x77, 0x7E, 0x7F, 0x89, 0x82, 0x7F, 0x82, 0x7F, 0x7C, 0x7E, 0x85, 0x87, 0x85, 0x7F, 0x82, 0x7E, 0x82, 0x7E, 0x7F, 0x8C, 0x7F, 0x7E, 0x73, 0x7F, 0x82, 0x82, 0x7E, 0x7C, 0x7C, 0x7F, 0x7F, 0x7F, 0x7E, 0x82, 0x7F, 0x7F, 0x7F, 0x7F, 0x87, 0x7F, 0x85, 0x7F, 0x82, 0x7C, 0x7C, 0x7F, 0x7F, 0x82, 0x7C, 0x82, 0x7E, 0x7F, 0x7C, 0x82, 0x87, 0x7E, 0x7E, 0x7C, 0x82, 0x82, 0x82, 0x7F, 0x79, 0x7E, 0x7E, 0x82, 0x85, 0x85, 0x7E, 0x7C, 0x7C, 0x85, 0x82, 0x89, 0x7F, 0x7F, 0x7F, 0x7F, 0x7E, 0x7F, 0x82, 0x85, 0x7F, 0x7F, 0x7C, 0x7E, 0x82, 0x7E, 0x82, 0x7C, 0x85, 0x7C, 0x85, 0x7E, 0x7F, 0x7C, 0x7C, 0x7F, 0x7F, 0x82, 0x82, 0x90, 0x85, 0x7E, 0x73, 0x7F, 0x90, 0x8E, 0x7F, 0x74, 0x73, 0x82, 0x7E, 0x7F, 0x8C, 0x7F, 0x7E, 0x70, 0x7F, 0x82, 0x85, 0x7F, 0x7C, 0x7C, 0x82, 0x7F, 0x82, 0x7F, 0x82, 0x7E, 0x7F, 0x7F, 0x7F, 0x7F, 0x7E, 0x7F, 0x7F, 0x82, 0x82, 0x7C, 0x85, 0x7E, 0x85, 0x7F, 0x7F, 0x7C, 0x7F, 0x7E, 0x82, 0x7F, 0x82, 0x7E, 0x7E, 0x85, 0x7F, 0x7E, 0x7C, 0x85, 0x82, 0x7F, 0x7F, 0x7F, 0x7E, 0x7E, 0x7C, 0x87, 0x7F, 0x85, 0x8C, 0x7C, 0x7E, 0x73, 0x85, 0x82, 0x7F, 0x8E, 0x82, 0x7F, 0x73, 0x79, 0x7C, 0x7E, 0x7F, 0x85, 0x7F, 0x7F, 0x79, 0x7F, 0x7E, 0x82, 0x85, 0x85, 0x82, 0x7E, 0x85, 0x7F, 0x7E, 0x7E, 0x85, 0x82, 0x7F, 0x7F, 0x7F, 0x99, 0x7F, 0x70, 0x63, 0x73, 0x9D, 0x87, 0x85, 0x6F, 0x7E, 0x94, 0x7F, 0x7C, 0x6C, 0x7F, 0x7F, 0x87, 0x82, 0x7F, 0x82, 0x7E, 0x7E, 0x7C, 0x7F, 0x7C, 0x7F, 0x85, 0x89, 0x7F, 0x7E, 0x7F, 0x85, 0x85, 0x7E, 0x7C, 0x7E, 0x85, 0x82, 0x7F, 0x7F, 0x82, 0x7E, 0x7E, 0x7C, 0x7F, 0x82, 0x85, 0x7E, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0xBF, 0x82, 0x50, 0x3B, 0x9B, 0xD3, 0x79, 0x50, 0x6F, 0xA0, 0xA2, 0x63, 0x73, 0x82, 0x8C, 0x82, 0x6F, 0x7F, 0x7F, 0x82, 0xCC, 0x7F, 0x51, 0x30, 0x8C, 0xC8, 0x73, 0x5E, 0x8C, 0xCC, 0x82, 0x48, 0x46, 0xA4, 0x97, 0x66, 0x6F, 0x8E, 0x97, 0x79, 0x66, 0x82, 0x7F, 0x89, 0x77, 0xB6, 0x93, 0x68, 0x4B, 0xA2, 0xC9, 0x97, 0x51, 0x6C, 0xA0, 0x7F, 0x5A, 0x54, 0x7E, 0xA6, 0x7F, 0x6C, 0x9D, 0x7F, 0x73, 0x4D, 0x90, 0xB2, 0x6A, 0x6F, 0x73, 0x9B, 0x82, 0x7E, 0xA0, 0x9D, 0x97, 0x5A, 0x61, 0xA0, 0x74, 0x66, 0x79, 0xB8, 0x7F, 0x26, 0x5A, 0xD5, 0xD5, 0x56, 0x46, 0x9D, 0xAD, 0x5A, 0x4D, 0x94, 0xA9, 0x7C, 0x6A, 0x94, 0x97, 0x73, 0x63, 0x82, 0x90, 0x7C, 0x7C, 0xA4, 0xAB, 0x87, 0x5A, 0x73, 0x7F, 0x5D, 0x4D, 0x73, 0xD5, 0xB8, 0x89, 0x74, 0x8E, 0x85, 0x37, 0x48, 0x99, 0xA6, 0x70, 0x66, 0x99, 0x7F, 0x68, 0x66, 0xA6, 0x93, 0x6F, 0x7C, 0x89, 0x7F, 0x61, 0x7E, 0xA9, 0x94, 0x7F, 0xA0, 0xA4, 0x99, 0x79, 0x90, 0x87, 0x54, 0x46, 0x61, 0x73, 0x85, 0x7E, 0x8C, 0xA6, 0x9D, 0x74, 0x66, 0x5A, 0x7C, 0x61, 0x85, 0x8C, 0xA0, 0x94, 0x87, 0x82, 0xA4, 0xC9, 0x93, 0x7F, 0x77, 0xAB, 0x6F, 0x2D, 0x77, 0x77, 0x8C, 0x42, 0x6A, 0x87, 0x73, 0x74, 0x6C, 0x9B, 0x8E, 0x77, 0x7F, 0x7F, 0x99, 0x7E, 0x9B, 0x99, 0x7F, 0x7F, 0x7F, 0xA9, 0x73, 0x7C, 0x79, 0x73, 0x68, 0x5E, 0x77, 0x6F, 0x77, 0x85, 0x99, 0x8E, 0x70, 0x7F, 0x7F, 0x6C, 0x6C, 0x79, 0x9B, 0x85, 0x79, 0x9D, 0xA2, 0x99, 0x6F, 0x87, 0x8C, 0x93, 0x6A, 0x5D, 0x5E, 0x63, 0x73, 0x74, 0x70, 0x89, 0x87, 0x97, 0x77, 0x89, 0x8E, 0x89, 0x85, 0x63, 0x7C, 0x7F, 0x89, 0x7C, 0x6F, 0x89, 0x7E, 0x77, 0x7E, 0x74, 0x7C, 0x63, 0x7F, 0x74, 0x70, 0x7C, 0x8C, 0x8C, 0x6F, 0x7C, 0x90, 0x8E, 0x90, 0x8C, 0x85, 0x74, 0x6C, 0x89, 0x82, 0x85, 0x87, 0x7F, 0x89, 0x74, 0x82, 0x7E, 0x85, 0x7F, 0x7F, 0x7F, 0x7F, 0x89, 0x7F, 0x7F, 0x8C, 0x90, 0x8E, 0x7C, 0x90, 0x7C, 0x87, 0x74, 0x8E, 0x7E, 0x87, 0x79, 0x7F, 0x82, 0x87, 0x85, 0x87, 0x8C, 0xA2, 0x8C, 0x8E, 0x87, 0x99, 0x89, 0x89, 0x7F, 0x87, 0x87, 0x79, 0x82, 0x6F, 0x87, 0x7C, 0x7E, 0x7F, 0x77, 0x7E, 0x70, 0x8C, 0x7F, 0x82, 0x87, 0x7F, 0x77, 0x82, 0x97, 0x82, 0x77, 0x79, 0x94, 0x7E, 0x73, 0x7F, 0x85, 0x7F, 0x7E, 0x90, 0x87, 0x6F, 0x7E, 0x8E, 0x87, 0x6F, 0x79, 0x8E, 0x7F, 0x70, 0x5D, 0x7C, 0x7F, 0x7F, 0x6A, 0x6A, 0x9B, 0x90, 0x93, 0x7C, 0x97, 0x94, 0x82, 0x77, 0x7E, 0x82, 0x82, 0x79, 0x82, 0x7E, 0x79, 0x7C, 0x89, 0x89, 0x74, 0x77, 0x8E, 0x8E, 0x74, 0x66, 0x90, 0x8C, 0x85, 0x66, 0x7F, 0x90, 0x87, 0x74, 0x79, 0x82, 0x85, 0x73, 0x7F, 0x7F, 0x87, 0x7F, 0x89, 0x85, 0x89, 0x85, 0x89, 0x79, 0x89, 0x87, 0x7E, 0x6F, 0x79, 0x85, 0x7C, 0x79, 0x87, 0x89, 0x73, 0x73, 0x7F, 0x89, 0x7E, 0x73, 0x7F, 0x7C, 0x7C, 0x7F, 0x93, 0x7F, 0x66, 0x82, 0x97, 0x82, 0x5A, 0x73, 0x8E, 0x82, 0x6C, 0x6C, 0x90, 0x7F, 0x6C, 0x6C, 0x7F, 0x7F, 0x68, 0x63, 0x85, 0x82, 0x74, 0x66, 0x74, 0x7F, 0x89, 0x7F, 0x77, 0x7C, 0x7F, 0x8C, 0x74, 0x7E, 0x82, 0x9B, 0x8C, 0x87, 0x82, 0x89, 0x82, 0x8C, 0x7E, 0x7F, 0x7F, 0x87, 0x87, 0x79, 0xAB, 0xB8, 0xAF, 0x87, 0x9D, 0xB2, 0xA4, 0x8E, 0x93, 0xA6, 0x9D, 0x89, 0x7F, 0x7C, 0x82, 0x77, 0x7F, 0x79, 0x73, 0x70, 0x6A, 0x70, 0x6F, 0x74, 0x6F, 0x6F, 0x6F, 0x70, 0x70, 0x77, 0x89, 0x87, 0x77, 0x77, 0x73, 0x79, 0x73, 0x7C, 0x7C, 0x68, 0x66, 0x68, 0x70, 0x68, 0x73, 0x74, 0x7E, 0x79, 0x79, 0x74, 0x77, 0x7F, 0x8C, 0x8C, 0x7C, 0x6C, 0x7E, 0x8E, 0x73, 0x6A, 0x73, 0x7C, 0x87, 0x8C, 0x6F, 0x54, 0x70, 0x97, 0x85, 0x66, 0x7F, 0x7F, 0x5D, 0x6C, 0x93, 0x97, 0x56, 0x6A, 0xAF, 0xDA, 0xBD, 0x99, 0xA6, 0xD5, 0xC5, 0xAF, 0x97, 0x93, 0x8C, 0x97, 0x85, 0x6F, 0x4B, 0x7C, 0x6F, 0x6F, 0x37, 0x51, 0x63, 0x7E, 0x73, 0x7C, 0x82, 0x8E, 0xA9, 0xB3, 0xAF, 0xA2, 0xA9, 0xB6, 0x94, 0x79, 0x6F, 0x7F, 0x6A, 0x5D, 0x44, 0x5D, 0x61, 0x5E, 0x54, 0x5E, 0x6F, 0x74, 0x70, 0x73, 0x70, 0x87, 0x87, 0x90, 0x7F, 0x77, 0x7F, 0x7F, 0x82, 0x79, 0x7F, 0x7E, 0x7E, 0x6A, 0x68, 0x5E, 0x63, 0x6A, 0x63, 0x68, 0x6A, 0x70, 0x79, 0x82, 0x82, 0x97, 0xA0, 0x8E, 0x74, 0x82, 0x9B, 0x8C, 0x6A, 0x6A, 0x74, 0x6C, 0x66, 0x74, 0xCC, 0xDA, 0xC4, 0x9D, 0xC1, 0xDA, 0xDA, 0xA9, 0x8E, 0xA6, 0xA6, 0x90, 0x7C, 0x70, 0x66, 0x6F, 0x6A, 0x63, 0x3E, 0x29, 0x66, 0x7E, 0x82, 0x4B, 0x5D, 0x7C, 0x9B, 0x94, 0xA0, 0xA6, 0xC4, 0xDA, 0xDA, 0x99, 0x7F, 0x85, 0xA9, 0x6A, 0x46, 0x3F, 0x5A, 0x5D, 0x48, 0x44, 0x46, 0x5D, 0x70, 0x82, 0x7C, 0x74, 0x7F, 0x90, 0x8E, 0x7E, 0x7E, 0x7E, 0x93, 0x85, 0x73, 0x63, 0x73, 0x7F, 0x6A, 0x54, 0x51, 0x5E, 0x5A, 0x5A, 0x66, 0x74, 0x6F, 0x74, 0x87, 0x7F, 0x7F, 0x89, 0x9D, 0x7E, 0x70, 0x7F, 0xA0, 0x97, 0x79, 0x6F, 0x70, 0x70, 0x73, 0x8C, 0xDA, 0xCC, 0xB6, 0x85, 0xDA, 0xDA, 0xD7, 0x97, 0x99, 0xB2, 0x90, 0x6A, 0x5E, 0x5E, 0x68, 0x61, 0x5E, 0x50, 0x4B, 0x61, 0x77, 0x70, 0x63, 0x6A, 0x8C, 0x94, 0x97, 0x93, 0xA2, 0x90, 0x9B, 0x99, 0xA2, 0xA4, 0xC1, 0xBD, 0x73, 0x56, 0x7F, 0x9D, 0x63, 0x26, 0x37, 0x63, 0x66, 0x4D, 0x58, 0x79, 0x90, 0x94, 0x94, 0x82, 0x6C, 0x82, 0x90, 0x87, 0x5D, 0x5A, 0x6C, 0x6C, 0x63, 0x61, 0x61, 0x70, 0x66, 0x6C, 0x63, 0x6A, 0x70, 0x6F, 0x73, 0x74, 0x7F, 0x6C, 0x79, 0x82, 0x9B, 0x87, 0x87, 0x87, 0x85, 0x6C, 0x66, 0x6A, 0x74, 0xD3, 0xDA, 0xDA, 0x93, 0x99, 0xDA, 0xDA, 0xA6, 0x6C, 0x82, 0xA6, 0x87, 0x66, 0x48, 0x3B, 0x3E, 0x5A, 0x70, 0x39, 0x28, 0x46, 0x93, 0xA0, 0x79, 0x7C, 0x97, 0xBF, 0xB3, 0xB6, 0x99, 0x93, 0x90, 0xA9, 0xA6, 0x90, 0x7E, 0x77, 0x82, 0x90, 0x79, 0x51, 0x44, 0x6C, 0x74, 0x66, 0x3F, 0x6A, 0x85, 0x99, 0x82, 0x79, 0x85, 0x89, 0x94, 0x7E, 0x6F, 0x54, 0x61, 0x63, 0x66, 0x5D, 0x56, 0x5E, 0x61, 0x68, 0x61, 0x6C, 0x6C, 0x82, 0x70, 0x74, 0x6C, 0x7C, 0x8C, 0x90, 0x79, 0x66, 0x66, 0x9D, 0x89, 0x70, 0x50, 0x70, 0x82, 0xA6, 0xDA, 0xDA, 0xD0, 0x7F, 0xDA, 0xDA, 0xDA, 0x79, 0x74, 0x7F, 0x99, 0x5A, 0x46, 0x26, 0x3E, 0x3E, 0x61, 0x50, 0x44, 0x35, 0x7E, 0xA6, 0xA6, 0x85, 0x99, 0xC1, 0xD0, 0xB8, 0xA2, 0x9B, 0x8E, 0x8C, 0x89, 0x87, 0x79, 0x5D, 0x63, 0x7C, 0x8C, 0x6F, 0x4B, 0x5A, 0x7F, 0x89, 0x73, 0x66, 0x7F, 0x97, 0x97, 0x7F, 0x7E, 0x74, 0x87, 0x7F, 0x73, 0x56, 0x48, 0x5A, 0x63, 0x66, 0x51, 0x56, 0x66, 0x6F, 0x6F, 0x6F, 0x70, 0x70, 0x79, 0x7C, 0x7E, 0x70, 0x7F, 0x87, 0x74, 0x73, 0x87, 0x82, 0x68, 0x56, 0x7E, 0x7F, 0x7E, 0x70, 0xDA, 0xDA, 0xDA, 0x50, 0xD3, 0xDA, 0xDA, 0x77, 0x7F, 0x7F, 0x85, 0x56, 0x50, 0x26, 0x26, 0x26, 0x56, 0x6F, 0x44, 0x35, 0x82, 0xC8, 0xC9, 0xA2, 0xAB, 0xC9, 0xD3, 0xC4, 0xAB, 0x9B, 0x74, 0x77, 0x7C, 0x85, 0x63, 0x44, 0x58, 0x74, 0x89, 0x74, 0x61, 0x6F, 0x7F, 0x94, 0x82, 0x77, 0x74, 0x94, 0x9B, 0x8C, 0x6C, 0x66, 0x7C, 0x77, 0x6A, 0x51, 0x51, 0x56, 0x5D, 0x5D, 0x56, 0x5E, 0x70, 0x7E, 0x6A, 0x58, 0x66, 0x6F, 0x7C, 0x7F, 0x8E, 0x7F, 0x6A, 0x7C, 0x7F, 0x7E, 0x63, 0x6A, 0x77, 0x7C, 0x7F, 0x97, 0xDA, 0xDA, 0xC8, 0x50, 0xD3, 0xDA, 0xDA, 0x73, 0x74, 0x7F, 0x79, 0x5A, 0x39, 0x26, 0x26, 0x26, 0x70, 0x73, 0x48, 0x37, 0x87, 0xD3, 0xDA, 0xC8, 0xB2, 0xC4, 0xCC, 0xCE, 0xAB, 0x90, 0x63, 0x68, 0x74, 0x7F, 0x51, 0x39, 0x4B, 0x70, 0x87, 0x79, 0x79, 0x77, 0x89, 0x9D, 0xA4, 0x87, 0x73, 0x79, 0x97, 0x87, 0x5E, 0x51, 0x6A, 0x7C, 0x66, 0x54, 0x4B, 0x58, 0x5E, 0x66, 0x61, 0x5D, 0x5E, 0x73, 0x77, 0x74, 0x5E, 0x79, 0x90, 0x7F, 0x5A, 0x66, 0xA2, 0x8C, 0x66, 0x4B, 0x77, 0x7F, 0x7F, 0x7E, 0xDA, 0xDA, 0xDA, 0x7F, 0xA6, 0xDA, 0xDA, 0x87, 0x74, 0x7F, 0x6F, 0x50, 0x3E, 0x26, 0x26, 0x26, 0x3F, 0x82, 0x63, 0x3F, 0x6F, 0xD3, 0xDA, 0xD3, 0xBA, 0xC8, 0xCC, 0xBA, 0xAB, 0x94, 0x6F, 0x48, 0x61, 0x7F, 0x66, 0x32, 0x37, 0x79, 0x9D, 0x87, 0x6C, 0x70, 0x90, 0xA2, 0xAB, 0x8C, 0x79, 0x74, 0x93, 0x85, 0x63, 0x50, 0x61, 0x7C, 0x68, 0x44, 0x3F, 0x50, 0x6C, 0x6F, 0x68, 0x68, 0x5A, 0x70, 0x7E, 0x85, 0x6A, 0x63, 0x68, 0x73, 0x7F, 0x74, 0x73, 0x66, 0x74, 0x7F, 0x7F, 0x7F, 0x7F, 0xDA, 0xDA, 0xDA, 0x7F, 0x7F, 0xDA, 0xDA, 0xB6, 0x70, 0x70, 0x5E, 0x54, 0x48, 0x2D, 0x26, 0x26, 0x2C, 0x99, 0x94, 0x63, 0x63, 0xBD, 0xDA, 0xDA, 0xC8, 0xAD, 0xB2, 0xC1, 0xC1, 0x93, 0x5A, 0x35, 0x51, 0x6F, 0x6F, 0x3F, 0x30, 0x5D, 0x90, 0xB2, 0xA9, 0x87, 0x7E, 0x93, 0xB8, 0x9D, 0x63, 0x54, 0x77, 0x8C, 0x6F, 0x50, 0x56, 0x61, 0x6C, 0x63, 0x5E, 0x46, 0x51, 0x6A, 0x7F, 0x6C, 0x50, 0x5A, 0x73, 0x87, 0x73, 0x63, 0x66, 0x77, 0x73, 0x74, 0x70, 0x7F, 0x7F, 0x7F, 0x7F, 0x85, 0xDA, 0xDA, 0xDA, 0x7F, 0xC8, 0xDA, 0xDA, 0x7C, 0x66, 0x6C, 0x54, 0x4B, 0x2D, 0x26, 0x26, 0x26, 0x6F, 0xAD, 0x7C, 0x5A, 0x7C, 0xDA, 0xDA, 0xDA, 0xB6, 0xAD, 0xB8, 0xB3, 0xA6, 0x68, 0x2D, 0x35, 0x61, 0x79, 0x4D, 0x37, 0x46, 0x8C, 0xA9, 0xB6, 0x9B, 0x8C, 0x90, 0xA9, 0xB3, 0x8C, 0x5D, 0x51, 0x70, 0x7C, 0x5A, 0x3F, 0x3F, 0x6A, 0x7E, 0x70, 0x61, 0x56, 0x63, 0x70, 0x74, 0x68, 0x50, 0x56, 0x77, 0x7E, 0x74, 0x5E, 0x61, 0x6F, 0x73, 0x7F, 0x7C, 0x7F, 0x7F, 0x7F, 0xB2, 0xDA, 0xDA, 0xD5, 0x97, 0xDA, 0xDA, 0xCC, 0x6C, 0x66, 0x61, 0x51, 0x3E, 0x26, 0x26, 0x26, 0x28, 0x87, 0xA2, 0x79, 0x6A, 0xBA, 0xDA, 0xDA, 0xDA, 0xB6, 0xAD, 0xB2, 0xAD, 0x9B, 0x5A, 0x26, 0x26, 0x66, 0x70, 0x3E, 0x28, 0x50, 0xAF, 0xCC, 0xC8, 0x99, 0x8E, 0xA2, 0xBF, 0xA0, 0x6F, 0x3E, 0x50, 0x66, 0x6F, 0x4D, 0x3F, 0x51, 0x6F, 0x7E, 0x6C, 0x5A, 0x56, 0x74, 0x87, 0x77, 0x51, 0x51, 0x73, 0x74, 0x6A, 0x50, 0x61, 0x6C, 0x7C, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0xDA, 0xDA, 0xDA, 0x9D, 0xA6, 0xDA, 0xDA, 0x8C, 0x6A, 0x6A, 0x39, 0x39, 0x26, 0x26, 0x26, 0x26, 0x50, 0xA9, 0xA6, 0x7F, 0x97, 0xDA, 0xDA, 0xDA, 0xC8, 0xB3, 0xA2, 0xA4, 0x99, 0x6F, 0x26, 0x26, 0x3F, 0x70, 0x5E, 0x32, 0x3F, 0x7E, 0xDA, 0xDA, 0xC4, 0x8C, 0x8E, 0xBD, 0xB2, 0x6F, 0x32, 0x37, 0x6A, 0x6C, 0x54, 0x39, 0x50, 0x70, 0x85, 0x7C, 0x5D, 0x48, 0x61, 0x7C, 0x7F, 0x56, 0x4D, 0x5E, 0x85, 0x79, 0x5E, 0x56, 0x68, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0xDA, 0xDA, 0xDA, 0xA2, 0x7F, 0xDA, 0xDA, 0xB6, 0x5A, 0x5A, 0x39, 0x42, 0x32, 0x26, 0x26, 0x26, 0x4D, 0xBA, 0xB2, 0x85, 0x85, 0xC8, 0xDA, 0xDA, 0xDA, 0xAB, 0x90, 0x9B, 0x9D, 0x77, 0x32, 0x26, 0x3F, 0x6A, 0x70, 0x3B, 0x4D, 0x9B, 0xD3, 0xD7, 0xAF, 0x97, 0x99, 0x9D, 0xA6, 0x74, 0x4D, 0x44, 0x61, 0x6F, 0x51, 0x44, 0x50, 0x70, 0x79, 0x73, 0x68, 0x56, 0x63, 0x6F, 0x74, 0x51, 0x48, 0x5E, 0x7F, 0x7E, 0x61, 0x5E, 0x6A, 0x7F, 0x7F, 0x7F, 0x7F, 0xDA, 0xDA, 0xDA, 0xC4, 0x7F, 0xDA, 0xDA, 0xB6, 0x68, 0x5A, 0x39, 0x26, 0x29, 0x26, 0x26, 0x26, 0x2D, 0x93, 0xB6, 0x9B, 0x97, 0xC8, 0xDA, 0xDA, 0xDA, 0xB3, 0x9D, 0x8C, 0x89, 0x6C, 0x37, 0x26, 0x2C, 0x61, 0x6C, 0x4B, 0x56, 0xAB, 0xD5, 0xDA, 0xB3, 0xA0, 0x97, 0x9D, 0x97, 0x73, 0x4D, 0x46, 0x56, 0x6A, 0x63, 0x54, 0x56, 0x6C, 0x79, 0x70, 0x61, 0x5D, 0x5E, 0x68, 0x6A, 0x5A, 0x54, 0x56, 0x66, 0x6C, 0x6F, 0x74, 0x74, 0x7C, 0x7F, 0x7F, 0xB2, 0xDA, 0xDA, 0xDA, 0xB2, 0xC8, 0xDA, 0xDA, 0x68, 0x54, 0x3E, 0x3B, 0x35, 0x32, 0x26, 0x26, 0x26, 0x82, 0xB8, 0x97, 0x85, 0xBD, 0xDA, 0xDA, 0xDA, 0xB8, 0x94, 0x93, 0x89, 0x7C, 0x48, 0x26, 0x26, 0x61, 0x7F, 0x66, 0x50, 0x7E, 0xC1, 0xD7, 0xD0, 0xB3, 0x94, 0x8C, 0x90, 0x93, 0x5E, 0x37, 0x42, 0x66, 0x77, 0x56, 0x4D, 0x5A, 0x74, 0x82, 0x74, 0x63, 0x56, 0x66, 0x73, 0x6A, 0x54, 0x4D, 0x58, 0x66, 0x68, 0x73, 0x74, 0x7F, 0x7F, 0x7F, 0xDA, 0xDA, 0xDA, 0xC4, 0x8C, 0xDA, 0xDA, 0xB6, 0x5A, 0x50, 0x39, 0x39, 0x30, 0x26, 0x26, 0x26, 0x44, 0xAD, 0xB3, 0x93, 0x93, 0xDA, 0xDA, 0xDA, 0xCC, 0xAB, 0x8C, 0x90, 0x87, 0x63, 0x28, 0x26, 0x32, 0x70, 0x7E, 0x61, 0x5D, 0x8E, 0xD0, 0xDA, 0xC8, 0x99, 0x89, 0x94, 0x9B, 0x70, 0x51, 0x3E, 0x63, 0x6F, 0x63, 0x51, 0x58, 0x6C, 0x77, 0x74, 0x6C, 0x58, 0x5D, 0x6A, 0x70, 0x6A, 0x50, 0x5D, 0x58, 0x73, 0x73, 0x73, 0x73, 0x74, 0x7F, 0xDA, 0xDA, 0xDA, 0xCC, 0xA2, 0xDA, 0xDA, 0xC1, 0x5A, 0x50, 0x39, 0x3F, 0x3B, 0x26, 0x26, 0x26, 0x3B, 0x9D, 0xAD, 0x93, 0x87, 0xBD, 0xDA, 0xDA, 0xDA, 0xA6, 0x8E, 0x82, 0x90, 0x7C, 0x3E, 0x26, 0x26, 0x6C, 0x7F, 0x6C, 0x56, 0x7E, 0xBF, 0xDA, 0xC8, 0x9D, 0x85, 0x89, 0xA0, 0x7F, 0x56, 0x30, 0x56, 0x73, 0x70, 0x54, 0x50, 0x6A, 0x7F, 0x85, 0x6F, 0x58, 0x58, 0x74, 0x70, 0x63, 0x4B, 0x5E, 0x66, 0x6A, 0x61, 0x6C, 0x74, 0x7F, 0x7F, 0xB2, 0xDA, 0xDA, 0xDA, 0xA2, 0xDA, 0xDA, 0xC1, 0x5A, 0x4B, 0x39, 0x48, 0x3B, 0x26, 0x26, 0x26, 0x3B, 0x93, 0xB2, 0x97, 0x93, 0xB8, 0xDA, 0xDA, 0xDA, 0xA6, 0x89, 0x85, 0x85, 0x74, 0x37, 0x26, 0x29, 0x63, 0x77, 0x79, 0x70, 0x89, 0xAD, 0xD5, 0xC5, 0x9B, 0x7F, 0x90, 0x93, 0x73, 0x54, 0x4D, 0x61, 0x70, 0x63, 0x5E, 0x56, 0x6C, 0x7C, 0x82, 0x6C, 0x5A, 0x5D, 0x6A, 0x6A, 0x5A, 0x54, 0x61, 0x6C, 0x6C, 0x5E, 0x68, 0x74, 0x7F, 0x85, 0xDA, 0xDA, 0xDA, 0xC9, 0xAD, 0xDA, 0xDA, 0xB3, 0x66, 0x44, 0x30, 0x35, 0x3F, 0x2D, 0x26, 0x26, 0x4D, 0xA2, 0xA6, 0x89, 0x87, 0xB6, 0xDA, 0xDA, 0xDA, 0xAB, 0x93, 0x85, 0x7F, 0x6A, 0x37, 0x26, 0x29, 0x61, 0x6F, 0x79, 0x7E, 0x90, 0xAF, 0xBF, 0xC8, 0xAF, 0x8C, 0x7F, 0x7F, 0x73, 0x58, 0x4D, 0x5E, 0x6F, 0x63, 0x61, 0x63, 0x73, 0x74, 0x7F, 0x77, 0x63, 0x58, 0x5E, 0x73, 0x6A, 0x61, 0x5E, 0x5A, 0x5E, 0x66, 0x77, 0x74, 0x70, 0x74, 0xB2, 0xDA, 0xDA, 0xDA, 0xB2, 0xB3, 0xDA, 0xDA, 0x90, 0x46, 0x39, 0x32, 0x54, 0x37, 0x26, 0x26, 0x26, 0x7F, 0xB8, 0xA0, 0x87, 0xA6, 0xDA, 0xDA, 0xDA, 0xAD, 0x8E, 0x8C, 0x94, 0x7E, 0x4D, 0x26, 0x26, 0x4B, 0x77, 0x6C, 0x5E, 0x87, 0xC1, 0xD0, 0xBD, 0xAB, 0xA9, 0x94, 0x7F, 0x74, 0x66, 0x46, 0x4D, 0x66, 0x79, 0x61, 0x5A, 0x6C, 0x85, 0x7F, 0x7E, 0x68, 0x5D, 0x51, 0x6A, 0x6A, 0x63, 0x50, 0x58, 0x66, 0x66, 0x70, 0x66, 0x74, 0x74, 0x7F, 0x9B, 0xDA, 0xDA, 0xDA, 0xB2, 0xB8, 0xDA, 0xDA, 0x90, 0x51, 0x44, 0x3E, 0x42, 0x2C, 0x26, 0x26, 0x26, 0x6A, 0xAD, 0xA0, 0x7F, 0x9B, 0xDA, 0xDA, 0xDA, 0xB8, 0x9D, 0x93, 0x90, 0x7E, 0x51, 0x29, 0x26, 0x44, 0x58, 0x61, 0x5D, 0x77, 0xA4, 0xCC, 0xD5, 0xC8, 0xAB, 0xA2, 0x97, 0x87, 0x5E, 0x51, 0x50, 0x61, 0x68, 0x5D, 0x5E, 0x5E, 0x7E, 0x87, 0x8E, 0x70, 0x66, 0x63, 0x6C, 0x74, 0x5E, 0x58, 0x56, 0x6F, 0x6F, 0x56, 0x51, 0x58, 0x73, 0x74, 0x7E, 0x7F, 0xA6, 0xDA, 0xDA, 0xDA, 0xB2, 0xCE, 0xDA, 0xDA, 0x87, 0x51, 0x44, 0x32, 0x42, 0x2C, 0x26, 0x26, 0x26, 0x6C, 0xAB, 0x9D, 0x89, 0xA9, 0xDA, 0xDA, 0xDA, 0xBD, 0x99, 0x94, 0x93, 0x82, 0x54, 0x26, 0x26, 0x39, 0x50, 0x5D, 0x56, 0x66, 0x90, 0xCE, 0xDA, 0xC4, 0x94, 0xA4, 0xBF, 0xB2, 0x6F, 0x4D, 0x54, 0x66, 0x61, 0x4D, 0x4D, 0x5A, 0x7E, 0x89, 0x8E, 0x74, 0x70, 0x7C, 0x7F, 0x73, 0x54, 0x5D, 0x6A, 0x73, 0x6A, 0x50, 0x5A, 0x5D, 0x66, 0x5A, 0x61, 0x74, 0x7F, 0x7F, 0xA6, 0xDA, 0xDA, 0xDA, 0xBA, 0xC4, 0xDA, 0xDA, 0x9B, 0x54, 0x48, 0x3E, 0x3F, 0x28, 0x26, 0x26, 0x26, 0x5E, 0x97, 0x9B, 0x87, 0x9B, 0xDA, 0xDA, 0xDA, 0xB8, 0xA2, 0xA0, 0x94, 0x7E, 0x56, 0x3B, 0x2C, 0x3B, 0x46, 0x3E, 0x39, 0x4D, 0x90, 0xB6, 0xC1, 0xC8, 0xCC, 0xD0, 0xC4, 0xB8, 0x9D, 0x6F, 0x54, 0x5A, 0x5E, 0x46, 0x35, 0x44, 0x73, 0x85, 0x87, 0x82, 0x7C, 0x7E, 0x7C, 0x82, 0x68, 0x5E, 0x5E, 0x74, 0x6F, 0x63, 0x5E, 0x5A, 0x6A, 0x6A, 0x61, 0x50, 0x56, 0x73, 0x7F, 0x7F, 0x9B, 0xDA, 0xDA, 0xDA, 0xB8, 0xAF, 0xDA, 0xDA, 0xB2, 0x5A, 0x48, 0x42, 0x48, 0x32, 0x26, 0x26, 0x26, 0x56, 0x97, 0x97, 0x79, 0x93, 0xCC, 0xDA, 0xDA, 0xC8, 0xA4, 0xAB, 0xAB, 0x8C, 0x63, 0x3E, 0x32, 0x3F, 0x4D, 0x3F, 0x2C, 0x3F, 0x70, 0x97, 0xB2, 0xB6, 0xDA, 0xDA, 0xC8, 0xAF, 0xC8, 0xB2, 0x79, 0x4B, 0x5E, 0x54, 0x3E, 0x28, 0x46, 0x5D, 0x73, 0x79, 0x82, 0x85, 0x89, 0x9D, 0x99, 0x7C, 0x61, 0x6C, 0x79, 0x6A, 0x5D, 0x63, 0x6F, 0x6F, 0x66, 0x63, 0x56, 0x56, 0x56, 0x5E, 0x68, 0x74, 0x7F, 0x85, 0xC8, 0xDA, 0xDA, 0xDA, 0xC4, 0xDA, 0xDA, 0xD0, 0x77, 0x5A, 0x54, 0x51, 0x37, 0x26, 0x26, 0x26, 0x32, 0x6A, 0x7F, 0x73, 0x74, 0xB6, 0xDA, 0xDA, 0xC8, 0xAB, 0xC4, 0xC5, 0xB2, 0x79, 0x58, 0x42, 0x4B, 0x4D, 0x48, 0x26, 0x26, 0x46, 0x79, 0x90, 0x7C, 0xAB, 0xDA, 0xDA, 0xD7, 0xA6, 0xA6, 0xBD, 0xAD, 0x77, 0x44, 0x42, 0x4B, 0x51, 0x48, 0x3F, 0x4B, 0x6C, 0x8E, 0x87, 0x82, 0x82, 0xA0, 0xA2, 0x93, 0x73, 0x6F, 0x6C, 0x79, 0x70, 0x6C, 0x61, 0x6A, 0x6A, 0x6F, 0x58, 0x51, 0x56, 0x66, 0x63, 0x5A, 0x61, 0x73, 0x7F, 0x85, 0xB2, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xD5, 0xDA, 0xB2, 0x5D, 0x50, 0x4D, 0x48, 0x26, 0x26, 0x26, 0x32, 0x51, 0x70, 0x7F, 0x7C, 0x9D, 0xC1, 0xDA, 0xDA, 0xC8, 0xBA, 0xBD, 0xB8, 0x9B, 0x77, 0x5D, 0x42, 0x42, 0x3F, 0x35, 0x29, 0x35, 0x56, 0x74, 0x7F, 0x87, 0xA2, 0xBD, 0xDA, 0xC9, 0xDA, 0xBD, 0xBA, 0x99, 0x79, 0x68, 0x4B, 0x4D, 0x4B, 0x54, 0x54, 0x54, 0x5E, 0x6C, 0x7E, 0x85, 0x82, 0x93, 0x9B, 0x9D, 0x93, 0x85, 0x7F, 0x82, 0x87, 0x82, 0x70, 0x66, 0x6C, 0x70, 0x70, 0x6A, 0x68, 0x61, 0x6A, 0x6A, 0x6C, 0x6A, 0x6A, 0x63, 0x6F, 0x79, 0x6F, 0x5D, 0x56, 0x7C, 0xA6, 0xC4, 0xDA, 0xDA, 0xDA, 0xB2, 0xB8, 0xDA, 0xDA, 0x90, 0x51, 0x74, 0x79, 0x5D, 0x2C, 0x26, 0x39, 0x44, 0x58, 0x5D, 0x68, 0x66, 0x94, 0xBD, 0xC4, 0xA2, 0x9D, 0xC5, 0xD7, 0xB6, 0x89, 0x74, 0x79, 0x73, 0x66, 0x54, 0x3F, 0x3B, 0x50, 0x63, 0x66, 0x56, 0x6A, 0x94, 0xAB, 0xA0, 0xB8, 0xC4, 0xD3, 0xAF, 0xA2, 0x9B, 0x97, 0x74, 0x61, 0x58, 0x5D, 0x5A, 0x48, 0x56, 0x5A, 0x66, 0x6C, 0x7C, 0x87, 0x89, 0x87, 0x8E, 0x90, 0x90, 0x85, 0x93, 0x7F, 0x8E, 0x7E, 0x8C, 0x74, 0x6F, 0x6C, 0x70, 0x73, 0x6C, 0x68, 0x6C, 0x70, 0x74, 0x77, 0x7C, 0x82, 0x7F, 0x7E, 0x79, 0x73, 0x6C, 0x63, 0x5A, 0x68, 0x70, 0x93, 0xBD, 0xDA, 0xD3, 0x94, 0xAB, 0xDA, 0xDA, 0x9B, 0x5D, 0x77, 0x97, 0x7C, 0x46, 0x28, 0x37, 0x46, 0x50, 0x5A, 0x50, 0x51, 0x7F, 0xB2, 0xAF, 0x8C, 0x8C, 0xC1, 0xDA, 0xBA, 0x8C, 0x87, 0x99, 0x93, 0x79, 0x63, 0x51, 0x58, 0x61, 0x66, 0x4D, 0x44, 0x5D, 0x79, 0x8C, 0x7F, 0x7F, 0x94, 0xA2, 0xAF, 0x99, 0x9D, 0x99, 0x9D, 0x93, 0x7F, 0x70, 0x6F, 0x7E, 0x79, 0x61, 0x56, 0x68, 0x90, 0x85, 0x74, 0x6C, 0x82, 0x89, 0x85, 0x74, 0x82, 0x7E, 0x8C, 0x82, 0x85, 0x7F, 0x90, 0x97, 0x90, 0x70, 0x68, 0x6F, 0x77, 0x73, 0x70, 0x74, 0x74, 0x6F, 0x6F, 0x77, 0x7F, 0x7F, 0x7F, 0x7E, 0x7F, 0x7F, 0x82, 0x7E, 0x6F, 0x68, 0x6A, 0x74, 0x70, 0x70, 0x7F, 0xAB, 0xCC, 0xD0, 0x89, 0x7F, 0xC4, 0xDA, 0xCC, 0x70, 0x7C, 0xA2, 0x94, 0x5D, 0x42, 0x50, 0x5D, 0x54, 0x4D, 0x4B, 0x4D, 0x6A, 0x8C, 0x90, 0x79, 0x85, 0xA2, 0xC9, 0xAF, 0xA0, 0x93, 0xA6, 0xA4, 0x8C, 0x74, 0x6A, 0x73, 0x70, 0x70, 0x54, 0x50, 0x5D, 0x77, 0x70, 0x63, 0x73, 0x90, 0xA2, 0x93, 0x8C, 0x99, 0x99, 0xA6, 0x90, 0x97, 0x87, 0x7F, 0x89, 0x82, 0x7E, 0x6A, 0x70, 0x73, 0x63, 0x5D, 0x5D, 0x70, 0x77, 0x79, 0x7C, 0x79, 0x85, 0x90, 0x93, 0x93, 0x7F, 0x93, 0x8E, 0x82, 0x73, 0x7E, 0x7E, 0x77, 0x68, 0x70, 0x74, 0x70, 0x77, 0x6F, 0x74, 0x70, 0x79, 0x82, 0x7F, 0x87, 0x7E, 0x85, 0x74, 0x74, 0x74, 0x79, 0x7C, 0x74, 0x79, 0x85, 0xB2, 0xCE, 0xA0, 0x7E, 0xAB, 0xDA, 0xC8, 0x79, 0x74, 0xA0, 0x9B, 0x79, 0x5D, 0x5E, 0x6A, 0x6F, 0x68, 0x50, 0x4B, 0x56, 0x87, 0x79, 0x6F, 0x73, 0x8E, 0xA4, 0x97, 0x8C, 0x94, 0xA9, 0xA6, 0x94, 0x87, 0x89, 0x8C, 0x8E, 0x7E, 0x6F, 0x61, 0x70, 0x77, 0x63, 0x58, 0x5E, 0x7C, 0x7E, 0x77, 0x74, 0x82, 0x90, 0x90, 0x89, 0x85, 0x93, 0x9D, 0x9D, 0x8E, 0x89, 0x90, 0x89, 0x82, 0x79, 0x7F, 0x77, 0x70, 0x68, 0x63, 0x68, 0x66, 0x74, 0x70, 0x7E, 0x7C, 0x85, 0x82, 0x85, 0x89, 0x8E, 0x8E, 0x7F, 0x7F, 0x7C, 0x87, 0x79, 0x77, 0x6F, 0x77, 0x77, 0x7E, 0x77, 0x79, 0x77, 0x7E, 0x77, 0x77, 0x79, 0x89, 0x7E, 0x77, 0x74, 0x7F, 0x85, 0x77, 0x77, 0x7F, 0x7E, 0x85, 0xA6, 0xC1, 0x89, 0x7F, 0xAD, 0xD7, 0xAD, 0x73, 0x93, 0xAD, 0x94, 0x74, 0x68, 0x73, 0x73, 0x74, 0x66, 0x54, 0x46, 0x66, 0x7F, 0x6C, 0x5D, 0x63, 0x93, 0x94, 0x89, 0x85, 0x9D, 0xA4, 0x9B, 0x90, 0x8E, 0x8C, 0x93, 0x90, 0x82, 0x77, 0x7E, 0x79, 0x70, 0x63, 0x6F, 0x6C, 0x7E, 0x68, 0x6C, 0x6C, 0x7F, 0x87, 0x7F, 0x7F, 0x8E, 0x99, 0x9D, 0x8E, 0x7F, 0x94, 0x94, 0x8E, 0x79, 0x79, 0x85, 0x77, 0x70, 0x5E, 0x6C, 0x6C, 0x73, 0x70, 0x77, 0x7C, 0x82, 0x7F, 0x82, 0x7E, 0x89, 0x87, 0x89, 0x7C, 0x7F, 0x82, 0x8E, 0x82, 0x7C, 0x6F, 0x77, 0x73, 0x7C, 0x79, 0x82, 0x7C, 0x73, 0x77, 0x7F, 0x87, 0x79, 0x7E, 0x79, 0x7E, 0x74, 0x87, 0x89, 0x6F, 0x6C, 0x87, 0x93, 0x7F, 0x7C, 0xB3, 0xB2, 0x89, 0x73, 0xAD, 0xAF, 0xA0, 0x7F, 0x9D, 0x93, 0x82, 0x7E, 0x7F, 0x74, 0x73, 0x7C, 0x70, 0x56, 0x66, 0x7C, 0x7E, 0x61, 0x6A, 0x7E, 0x7E, 0x79, 0x7C, 0x90, 0x8C, 0x89, 0x97, 0x93, 0x94, 0x89, 0x93, 0x87, 0x7F, 0x79, 0x7F, 0x7F, 0x7E, 0x7C, 0x77, 0x7C, 0x74, 0x6C, 0x6C, 0x73, 0x7F, 0x7C, 0x7E, 0x7F, 0x82, 0x82, 0x7F, 0x93, 0x90, 0x8C, 0x85, 0x87, 0x8E, 0x7F, 0x7C, 0x77, 0x85, 0x7F, 0x7C, 0x74, 0x70, 0x74, 0x6A, 0x79, 0x73, 0x82, 0x7E, 0x7F, 0x77, 0x7F, 0x85, 0x87, 0x7F, 0x85, 0x7F, 0x7F, 0x7C, 0x7F, 0x79, 0x7F, 0x7C, 0x82, 0x74, 0x79, 0x77, 0x7F, 0x7E, 0x7C, 0x7E, 0x7C, 0x7E, 0x79, 0x7F, 0x87, 0x79, 0x74, 0x79, 0x7F, 0x7F, 0x77, 0x8E, 0x99, 0x93, 0x7F, 0x8C, 0xA2, 0x93, 0x97, 0x89, 0xA6, 0x85, 0x85, 0x89, 0x93, 0x90, 0x79, 0x79, 0x6F, 0x7F, 0x79, 0x7E, 0x70, 0x73, 0x79, 0x6C, 0x6F, 0x6F, 0x8C, 0x82, 0x7F, 0x73, 0x7F, 0x82, 0x7F, 0x90, 0x8E, 0x82, 0x85, 0x82, 0x8E, 0x7F, 0x7F, 0x87, 0x7F, 0x7F, 0x7F, 0x7F, 0x7C, 0x77, 0x79, 0x7F, 0x74, 0x7F, 0x73, 0x7E, 0x70, 0x93, 0x8E, 0x7F, 0x7E, 0x89, 0x8E, 0x7F, 0x79};

char* audio_data = &_binary_tune_wav_start;

static void pause(int t) {
  // Pause for about t ms
  int i;
  for (;t>0;t--) {
    for (i=5000;i>0;i--) dummy(i);
  }
}


static void audio_init(void)
{
  //vals read from raspbian:
  //PWMCLK_CNTL = 148 = 10010100
  //PWMCLK_DIV = 16384
  //PWM_CONTROL=9509 = 10010100100101
  //PWM0_RANGE=1024
  //PWM1_RANGE=1024

  unsigned int range = 0x400;
  unsigned int idiv = 2;
  /* unsigned int pwmFrequency = (19200000 / idiv) / range; */

  SET_GPIO_ALT(40, 0);
  SET_GPIO_ALT(45, 0);
  pause(2);

  *(clk + BCM2835_PWMCLK_CNTL) = PM_PASSWORD | (1 << 5);    // stop clock
  *(clk + BCM2835_PWMCLK_DIV)  = PM_PASSWORD | (idiv<<12);  // set divisor
  *(clk + BCM2835_PWMCLK_CNTL) = PM_PASSWORD | 16 | 1;      // enable + oscillator (raspbian has this as plla)

  pause(2); 

  // disable PWM
  *(pwm + BCM2835_PWM_CONTROL) = 0;
       
  pause(2);

  *(pwm+BCM2835_PWM0_RANGE) = range;
  *(pwm+BCM2835_PWM1_RANGE) = range;

  *(pwm+BCM2835_PWM_CONTROL) =
    BCM2835_PWM1_USEFIFO | // Use FIFO and not PWM mode
                           //          BCM2835_PWM1_REPEATFF |
    BCM2835_PWM1_ENABLE  | // enable channel 1
    BCM2835_PWM0_USEFIFO | // use FIFO and not PWM mode
                           //          BCM2835_PWM0_REPEATFF |  */
    1<<6                 | // clear FIFO
    BCM2835_PWM0_ENABLE;   // enable channel 0

  pause(2);
}

void
audio_test()
{
  int i=0;
  long status;
  audio_init();

  while (i < 1000000) {
	DISABLE_IRQ();
    status =  *(pwm + BCM2835_PWM_STATUS);
    if (!(status & BCM2835_FULL1)) {
      *(pwm+BCM2835_PWM_FIFO) = audio_data[i];
      //      *(pwm+BCM2835_PWM_FIFO) = audio_data[i];
      i++;
    }
    if ((status & ERRORMASK)) {
      //                uart_print("error: ");
      //                hexstring(status);
      //                uart_print("\r\n");
      *(pwm+BCM2835_PWM_STATUS) = ERRORMASK;
    }
    set_tick_and_enable_timer();
    ENABLE_IRQ();
  }
}
