<html>
<head>
<title>Version</title>
</head>
<body>

<script language=javascript type=text/javascript>
function stopRKey(evt) {
var evt = (evt) ? evt : ((event) ? event : null);
var node = (evt.target) ? evt.target :((evt.srcElement) ? evt.srcElement : null);
if ((evt.keyCode == 13) && (node.type=="text"))
{return false;}
}

document.onkeypress = stopRKey;
</script>
<?php
if($_POST['saved'] == 1) {
if($_POST['filepath'] != "files/") {
echo "<font color='red'>SETTINGS SAVED TO FILE</font><br/><br/>"; }
else {
echo "<font color='red'>NO FILE SELECTED YET.. PLEASE DO SO </font><a href='SaveSettings.php'>HERE</a><br/><br/>"; }
}
?>

<form method='post' action='Version.php'>

<h2>Version</h2>

The settings on this page should not be changed by the ordinary user,
but appear here for documentation purposes, and so that they can
form part of the standard databases and be queried accordingly.

<p/><code>parm&nbsp; </code><strong> Pythia:versionNumber &nbsp;</strong> 
 (<code>default = <strong>8.100</strong></code>)<br/>
Version and subversion number, with three significant decimals.
</parmfix>
<p/><code>mode&nbsp; </code><strong> Pythia:versionDate &nbsp;</strong> 
 (<code>default = <strong>20071020</strong></code>)<br/>
Last date of change of current (sub)version, in format yyyymmdd.
</modefix>

</body>
</html>

<!-- Copyright (C) 2007 Torbjorn Sjostrand -->
