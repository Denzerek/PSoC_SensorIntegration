

#include "glcdPrints.h"
#include "lcdDriver.h"
#include "newHavenLCD.h"



const LCDChar_s LCDCharMap[]={
    {' ',{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},    //  space
    {'!',{0x00, 0x00, 0x2f, 0x00, 0x00, 0x00}},    //  !
    {'"',{0x00, 0x07, 0x00, 0x07, 0x00, 0x00}},    //  "
    {'#',{0x14, 0x7f, 0x14, 0x7f, 0x14, 0x00}},    //  #
    {'$',{0x24, 0x2a, 0x7f, 0x2a, 0x12, 0x00}},    //  $
    {'%',{0x23, 0x13, 0x08, 0x64, 0x62, 0x00}},    //  %
    {'&',{0x36, 0x49, 0x55, 0x22, 0x50, 0x00}},    //  &
    {'\'',{0x00, 0x05, 0x03, 0x00, 0x00, 0x00}},    //  '
    {'(',{0x00, 0x1c, 0x22, 0x41, 0x00, 0x00}},    //  (
    {')',{0x00, 0x41, 0x22, 0x1c, 0x00, 0x00}},    //  )
    {'*',{0x14, 0x08, 0x3E, 0x08, 0x14, 0x00}},    //  *
    {'+',{0x08, 0x08, 0x3E, 0x08, 0x08, 0x00}},    //  +
    {',',{0x00, 0x00, 0xA0, 0x60, 0x00, 0x00}},    //  ,
    {'-',{0x08, 0x08, 0x08, 0x08, 0x08, 0x00}},    //  -
    {'.',{0x00, 0x60, 0x60, 0x00, 0x00, 0x00}},    //  .
    {'/',{0x20, 0x10, 0x08, 0x04, 0x02, 0x00}},    //  /

    {'0',{0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00}},    //  0
    {'1',{0x00, 0x42, 0x7F, 0x40, 0x00, 0x00}},    //  1
    {'2',{0x42, 0x61, 0x51, 0x49, 0x46, 0x00}},    //  2
    {'3',{0x21, 0x41, 0x45, 0x4B, 0x31, 0x00}},    //  3
    {'4',{0x18, 0x14, 0x12, 0x7F, 0x10, 0x00}},    //  4
    {'5',{0x27, 0x45, 0x45, 0x45, 0x39, 0x00}},    //  5
    {'6',{0x3C, 0x4A, 0x49, 0x49, 0x30, 0x00}},    //  6
    {'7',{0x01, 0x71, 0x09, 0x05, 0x03, 0x00}},    //  7
    {'8',{0x36, 0x49, 0x49, 0x49, 0x36, 0x00}},    //  8
    {'9',{0x06, 0x49, 0x49, 0x29, 0x1E, 0x00}},    //  9

    {':',{0x00, 0x36, 0x36, 0x00, 0x00, 0x00}},    //  :
    {';',{0x00, 0x56, 0x36, 0x00, 0x00, 0x00}},    //  ;
    {'<',{0x08, 0x14, 0x22, 0x41, 0x00, 0x00}},    //  <
    {'=',{0x14, 0x14, 0x14, 0x14, 0x14, 0x00}},    //  =
    {'>',{0x00, 0x41, 0x22, 0x14, 0x08, 0x00}},    //  >
    {'?',{0x02, 0x01, 0x51, 0x09, 0x06, 0x00}},    //  ?
    {'@',{0x32, 0x49, 0x59, 0x51, 0x3E, 0x00}},    //  @
//
    {'A',{0x7C, 0x12, 0x11, 0x12, 0x7C, 0x00}},    //  A
    {'B',{0x7F, 0x49, 0x49, 0x49, 0x36, 0x00}},    //  B
    {'C',{0x3E, 0x41, 0x41, 0x41, 0x22, 0x00}},    //  C
    {'D',{0x7F, 0x41, 0x41, 0x22, 0x1C, 0x00}},    //  D
    {'E',{0x7F, 0x49, 0x49, 0x49, 0x41, 0x00}},    //  E
    {'F',{0x7F, 0x09, 0x09, 0x09, 0x01, 0x00}},    //  F
    {'G',{0x3E, 0x41, 0x49, 0x49, 0x7A, 0x00}},    //  G
    {'H',{0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00}},    //  H
    {'I',{0x00, 0x41, 0x7F, 0x41, 0x00, 0x00}},    //  I
    {'J',{0x20, 0x40, 0x41, 0x3F, 0x01, 0x00}},    //  J
    {'K',{0x7F, 0x08, 0x14, 0x22, 0x41, 0x00}},    //  K
    {'L',{0x7F, 0x40, 0x40, 0x40, 0x40, 0x00}},    //  L
    {'M',{0x7F, 0x02, 0x0C, 0x02, 0x7F, 0x00}},    //  M
    {'N',{0x7F, 0x04, 0x08, 0x10, 0x7F, 0x00}},    //  N
    {'O',{0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00}},    //  O
    {'P',{0x7F, 0x09, 0x09, 0x09, 0x06, 0x00}},    //  P
    {'Q',{0x3E, 0x41, 0x51, 0x21, 0x5E, 0x00}},    //  Q
    {'R',{0x7F, 0x09, 0x19, 0x29, 0x46, 0x00}},    //  R
    {'S',{0x46, 0x49, 0x49, 0x49, 0x31, 0x00}},    //  S
    {'T',{0x01, 0x01, 0x7F, 0x01, 0x01, 0x00}},    //  T
    {'U',{0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00}},    //  U
    {'V',{0x1F, 0x20, 0x40, 0x20, 0x1F, 0x00}},    //  V
    {'W',{0x3F, 0x40, 0x38, 0x40, 0x3F, 0x00}},    //  W
    {'X',{0x63, 0x14, 0x08, 0x14, 0x63, 0x00}},    //  X
    {'Y',{0x07, 0x08, 0x70, 0x08, 0x07, 0x00}},    //  Y
    {'Z',{0x61, 0x51, 0x49, 0x45, 0x43, 0x00}},    //  Z
//
    {'[',{0x00, 0x7F, 0x41, 0x41, 0x00, 0x00}},    //  [
    {']',{0x00, 0x41, 0x41, 0x7F, 0x00, 0x00}},    //  ]
    {'^',{0x04, 0x02, 0x01, 0x02, 0x04, 0x00}},    //  ^
    {'_',{0x40, 0x40, 0x40, 0x40, 0x40, 0x00}},    //  _
//
    {'a',{0x20, 0x54, 0x54, 0x54, 0x78, 0x00}},    //  a
    {'b',{0x7F, 0x48, 0x44, 0x44, 0x38, 0x00}},    //  b
    {'c',{0x38, 0x44, 0x44, 0x44, 0x20, 0x00}},    //  c
    {'d',{0x38, 0x44, 0x44, 0x48, 0x7F, 0x00}},    //  d
    {'e',{0x38, 0x54, 0x54, 0x54, 0x18, 0x00}},    //  e
    {'f',{0x08, 0x7E, 0x09, 0x01, 0x02, 0x00}},    //  f
    {'g',{0x18, 0xA4, 0xA4, 0xA4, 0x7C, 0x00}},    //  g
    {'h',{0x7F, 0x08, 0x04, 0x04, 0x78, 0x00}},    //  h
    {'i',{0x00, 0x44, 0x7D, 0x40, 0x00, 0x00}},    //  i
    {'j',{0x40, 0x80, 0x84, 0x7D, 0x00, 0x00}},    //  j
    {'k',{0x7F, 0x10, 0x28, 0x44, 0x00, 0x00}},    //  k
    {'l',{0x00, 0x41, 0x7F, 0x40, 0x00, 0x00}},    //  l
    {'m',{0x7C, 0x04, 0x18, 0x04, 0x78, 0x00}},    //  m
    {'n',{0x7C, 0x08, 0x04, 0x04, 0x78, 0x00}},    //  n
    {'o',{0x38, 0x44, 0x44, 0x44, 0x38, 0x00}},    //  o
    {'p',{0xFC, 0x24, 0x24, 0x24, 0x18, 0x00}},    //  p
    {'q',{0x18, 0x24, 0x24, 0x18, 0xFC, 0x00}},    //  q
    {'r',{0x7C, 0x08, 0x04, 0x04, 0x08, 0x00}},    //  r
    {'s',{0x48, 0x54, 0x54, 0x54, 0x20, 0x00}},    //  s
    {'t',{0x04, 0x3F, 0x44, 0x40, 0x20, 0x00}},    //  t
    {'u',{0x3C, 0x40, 0x40, 0x20, 0x7C, 0x00}},    //  u
    {'v',{0x1C, 0x20, 0x40, 0x20, 0x1C, 0x00}},    //  v
    {'w',{0x3C, 0x40, 0x30, 0x40, 0x3C, 0x00}},    //  w
    {'x',{0x44, 0x28, 0x10, 0x28, 0x44, 0x00}},    //  x
    {'y',{0x1C, 0xA0, 0xA0, 0xA0, 0x7C, 0x00}},    //  y
    {'z',{0x44, 0x64, 0x54, 0x4C, 0x44, 0x00}},    //  z
//
    {'{',{0x00, 0x10, 0x7C, 0x82, 0x00, 0x00}},    //  {
    {'|',{0x00, 0x00, 0xFF, 0x00, 0x00, 0x00}},    //  |
    {'}',{0x00, 0x82, 0x7C, 0x10, 0x00, 0x00}},    //  }
    };


const uint32_t lengthofLCDCharMapMap = sizeof( LCDCharMap ) / sizeof( LCDChar_s );

