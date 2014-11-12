function exec_cmd($command, $timeout, &$cmd_output, &$cmd_ret_val)
{
	// if timeout is not set or 0, use default
	if($timeout == 0 || $timeout == null){
		$timeout = 5;	
	}
	// the return value of the function
	// default: -2: Command takes more time than timeout
	$ret_val = -2;
	// file descriptors passed to the process	
	$descriptors = array(
		0 => array('pipe', 'r'), // stdin
		1 => array('pipe', 'w'), // stdout
		2 => array('pipe', 'w')  // stderr
	);

	// start the command
	// TODO: 2>&1?
	$process = proc_open($command, $descriptors, $pipes);
	// check whether proc_open is successful
	if( !is_resource($process) ){
		return -1; // run command failed
	}
	
	// set stdout of command to non-blocking
	// so when reading from the pipe, it won't stuck
	stream_set_blocking($pipes[1], 0);

	// convert timeout to microseconds
	$timeout *= 1000000;
	
	// while we still have time to wait
	while($timeout > 0) {
		// track the beginning time	
		$start = microtime(true);

		// wait for output or the timer expired
		$read  = array($pipes[1]);
		$other = array(); // empty array
		stream_select($read, $other, $other, 0, $timeout);
		
		// get the status of the process
		$status = proc_get_status($process);

		// get the contents from the stdout of command
		// append to the cmd_output
		$cmd_output .= stream_get_contents($pipes[1]);
		// check the status of the command is running or not
		if(!$status['running']){
			// the command is not running anymore 
			// save the return code of the command
			$cmd_ret_val = $status['exitcode']; 
			$ret_val = 0; // command finishes before time expires
			break;
		}
		// update the timeout value
		$timeout -= (microtime(true) - $start) * 1000000;

		// what about the errors in pipe #2?
	}

	// command finishes or time expires
	// send SIGKILL to command in case it is still running
	proc_terminate($process, 9);
	// close the open streams and process
	foreach ($pipes as $pipe){
		fclose($pipe);
	}
	proc_close($process);	
	/**
	* http://www.tldp.org/LDP/abs/html/exitcodes.html
	* 1	Catchall for general errors	let "var1 = 1/0"
  * 2	Misuse of shell builtins (according to Bash documentation)
	* 126	Command invoked cannot execute
	* 127	"command not found"	illegal_command	Possible problem with $PATH or a typo
	* 128	Invalid argument to exit
	* 128+n	Fatal error signal "n"	kill -9 $PPID of script	$? returns 137 (128 + 9)
	* 130	Script terminated by Control-C	Ctl-C	Control-C is fatal error signal 2, (130 = 128 + 2, see above)
	* 255	Exit status out of range	exit -1	exit takes only integer args in the range 0 - 255
	*/
	if($cmd_ret_val == 126 || $cmd_ret_val == 127 || $cmd_ret_val == 128 || $cmd_ret_val == 255){
		$ret_val = -1;
	}

	// remove the last character of the output
	$cmd_output = rtrim($cmd_output);

	return $ret_val;
}
