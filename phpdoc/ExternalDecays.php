<html>
<head>
<title>External Decays</title>
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

<form method='post' action='ExternalDecays.php'>

<h2>External Decays</h2>

<code>DecayHandler</code> is a base class for the external handling of 
decays. It is intended for normal particle decays, primarily 
<i>B</i> mesons and <i>tau</i>, and cannot be used to redirect
decays of heavy resonances like <i>t</i> or <i>Z^0</i>.  
The user-written derived class is called if a pointer to it has
been given with the <?php $filepath = $_GET["filepath"];
echo "<a href='ProgramFlow.php?filepath=".$filepath."' target='page'>";?>
<code>pythia.decayPtr()</code></a> 
method, where it also is specified which particles it will be called for. 
This particle information is accessible with the
<?php $filepath = $_GET["filepath"];
echo "<a href='ParticleDataScheme.php?filepath=".$filepath."' target='page'>";?><code>externalDecay()</code></a>
method. 

<p/>
There is only one pure virtual method in <code>DecayHandler</code>, 
to do the decay: 
<p/><code>method&nbsp; </code><strong> virtual bool decay(vector&lt;int&gt;&amp; idProd, vector&lt;double&gt;&amp; mProd, vector&lt;Vec4&gt;&amp; pProd,  &nbsp;</strong> <br/>
int iDec, const Event&amp; event)">
where 
<br/><code>argument</code><strong> idProd </strong>  :  is a list of particle PDG identity codes,
  
<br/><code>argument</code><strong> mProd </strong>  :  is a list of their respective masses (in GeV), and
  
<br/><code>argument</code><strong> pProd </strong>  :  is a list of their respective four-momenta.
  
  

<p/>
At input, these vectors each have size one, so that <code>idProd[0]</code>, 
<code>mProd[0]</code> and <code>pProd[0]</code> contains information on the 
particle that is to be decayed. At output, the vectors should have 
increased by the addition of all the decay products. Even if initially 
defined in the rest frame of the mother, the products should have been 
boosted so that their four-momenta add up to the <code>pProd[0]</code> of 
the decaying particle. 

<p/>
Should it be of interest to know the prehistory of the decaying 
particle, e.g. to set some helicity information affecting the 
decay angular distribution, the full event record is available 
read-only, with info in which slot <code>iDec</code> the decaying particle 
is stored.

<p/>
The routine should return <code>true</code> if it managed the decay and 
<code>false</code> otherwise, in which case <code>Pythia</code> will try 
to do the decay itself.  

<p/>
Note that the decay vertex is always set by <code>Pythia</code>, and that 
<i>B-Bbar</i> oscillations have already been taken into account, 
if they were switched on. Thus <code>idProd[0]</code> may be the opposite 
of <code>event[iDec].id()</code>, where the latter provides the code at 
production.

<p/>
A sample test program is available in <code>main17.cc</code>, providing 
a simple example of how to use this facility.

</body>
</html>

<!-- Copyright C 2007 Torbjorn Sjostrand -->
