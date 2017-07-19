<!DOCTYPE HTML>
<?php  
$light = $_GET['mode'];
if($light == "day") {  
  $file = fopen("urlWrite.json", "w") or die("can't open file");
  fwrite($file, '{"mode": "day"}');
  fclose($file);
} 
else if ($light == "night") {  
  $file = fopen("urlWrite.json", "w") or die("can't open file");
  fwrite($file, '{"mode": "night"}');
  fclose($file);
}
?>
<html>
	<head>
		<link type="text/css" rel="stylesheet" href="stylesheetLite.css"/>
		<link rel="shortcut icon" href="favicon.ico" type="image/x-icon">
		<title>Team Cosmo Light Control</title>
	</head>
	<body>
		<h1>Welcome to Light Control</h1>
		<?php
            if($light=="day") {
              echo("Day mode is on");
            }
            else if ($light=="night") {
              echo("Night mode is on");
            }
            else {
              echo ("Do something.");
            }
          ?>
		<div class="reset"><a href="?mode=off" >Off</a></div>
		<div class="rainbow"><a href="?mode=rainbow" >Turn Rainbow Mode On</a></div>
		<div class="night"><a href="?mode=night" >Turn Night Mode On</a></div>
		<div class="day"><a href="?mode=day" >Turn Day Mode On</a></div>
		<div class="purple"><a href="?mode=purple" >Turn Purple Mode On</a></div>
		<div class="hex"><a href="?mode=hex" >Change Colors Using Hexadecimal</a></div>
	</body>
</html>	