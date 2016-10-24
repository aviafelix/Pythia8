<html>
<head>
<title>Front</title>
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

<form method='post' action='Frontpage.php'>

<h1>PYTHIA 8</h1>

<h2>Welcome to PYTHIA - The Lund Monte Carlo!</h2>

<p/>
PYTHIA 8 is still incomplete, in particular with respect to the 
intended process library, and not yet tuned up for production runs. 
What is offered here is a snapshot, to allow testing and feedback. 
The program is under active development, however, and is intended to 
replace the existing PYTHIA 6 as the official version by the end of 2007.

<p/>
Use the left-hand index to navigate in the existing documentation.
Also note that there is a separate
<a href="pythia8080.pdf" target="page"><b>Brief Introduction</b></a>
(as a pdf file), 
that offers the best way to get to understand the basic structure of 
PYTHIA 8. The current pages offer the more detailed picture, 
with the corresponding danger of then losing overview.  

<p/>
The complete <b>PYTHIA 6.4 Physics and Manual</b>, published in<br/>
<b>T. Sj&ouml;strand, S. Mrenna and P. Skands, JHEP05 (2006) 026</b>,<br/>
in detail describes the physics (partly) implemented also in 
PYTHIA 8. It therefore is the main reference that you should
quote, whether you use PYTHIA 6 or PYTHIA 8.

<p/>
Main author: <b>Torbj&ouml;rn Sj&ouml;strand</b><br/> 
CERN/PH, CH-1211 Geneva, Switzerland, and<br/>
Department of Theoretical Physics, Lund University, 
SE-223 62 Lund, Sweden<br/>
phone: + 41 - 22 - 767 82 27, e-mail: torbjorn@thep.lu.se   

<p/>
Author: <b>Stephen Mrenna</b><br/> 
Computing Division, Simulations Group, 
Fermi National Accelerator Laboratory,<br/>
MS 234, Batavia, IL 60510, USA<br/>
phone: + 1 - 630 - 840 - 2556, e-mail: mrenna@fnal.gov

<p/>
Author: <b>Peter Skands</b><br/> 
Theoretical Physics Department,
Fermi National Accelerator Laboratory,<br/>
MS 106, Batavia, IL 60510, USA<br/> 
phone: + 1 - 630 - 840 - 2270, e-mail: skands@fnal.gov 

<p/>
Makefiles, configure scripts and HepMC interface by <b>Mikhail Kirsanov</b>.

<p/>
Conversion to PHP files by <b>Ben Lloyd</b>.

<p/>
The program and the documentation is
Copyright &copy; 2007 Torbj&ouml;rn Sj&ouml;strand 
 

</body>
</html>

<!-- Copyright C 2007 Torbjorn Sjostrand -->
