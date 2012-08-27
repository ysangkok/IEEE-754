<?php
  $bi = $_SERVER["argv"][1];
  $be = $_SERVER["argv"][2];
  $bf = $_SERVER["argv"][3];

  $time = (string) substr(number_format(microtime(true),2),-6);

  $result = array();

  exec("./btd2 $bi $be $bf", $result);
  $arr = join("\n",$result);
/*
  $arr = array( 'di' => $result[0], 'df' => $result[1], 'de' => $result[2], 
                'dr' => $result[3], 'drs' => $result[4], 'br' => $result[5], 
                'brs' => $result[6], 'bi' => $result[7], 'bf' => $result[8], 'be' =>$result[9]);
*/

  file_put_contents("$time.org.txt",$arr); 

/*
  system("curl -s -d binary_exponent=$be -d binary_fraction=$bf -d binary_integer=$bi \"http://babbage.cs.qc.cuny.edu/IEEE-754/computeDec.php\" > $time.official.txt");
  system("cmp $time.official.txt $time.org.txt", $resultvar);
  if ($resultvar != 0) {
	echo "Official $time != ours!\n";
	exit(1);
  }
*/

/*
  $cpp = `./btd  $bi $be $bf`;
  $c =   `./btd2 $bi $be $bf`;
  if ($cpp != $c) {
	echo "C $time != C++!\n";
	print_r(array("cpp" => $cpp, "c" => $c));
	exit(1);
  }
*/

  unset($result, $arr);
  $result = array();
  exec("node btd2-unopt.js $bi $be $bf", $result);
/*
  $arr = array( 'di' => $result[0], 'df' => $result[1], 'de' => $result[2], 
                'dr' => $result[3], 'drs' => $result[4], 'br' => $result[5], 
                'brs' => $result[6], 'bi' => $result[7], 'bf' => $result[8], 'be' =>$result[9]);
*/
  $arr = join("\n",$result);
  file_put_contents("$time.emscripten.txt",$arr); 

  echo "ORG RIGHT!\n";
  system("cmp $time.emscripten.txt $time.org.txt",$resultvar);
  if ($resultvar != 0) {
    echo "emscripten differs! time $time\n";

    system("fold -w 20 < $time.emscripten.txt > $time.emscripten.formatted.txt");
    system("fold -w 20 < $time.org.txt > $time.org.formatted.txt");
    system("diff -d -a $time.*.formatted.txt");
    exit(1);
  }
 ?>
