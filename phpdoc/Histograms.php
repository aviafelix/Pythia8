<html>
<head>
<title>Histograms</title>
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

<form method='post' action='Histograms.php'>
    
<h2>Histograms</h2>

The <code>Hist</code> class gives a simple implementation of 
one-dimensional histograms, useful for quick-and-dirty testing, 
without the need to link to more sophisticated packages. 
A Histogram is declared by a    
<p/><code>class&nbsp; </code><strong> Hist name( title, numberOfBins, xMin, xMax) &nbsp;</strong> <br/>
where
<br/><code>argument</code><strong> title </strong>  :  
is a string with the title of the histogram at output,
  
<br/><code>argument</code><strong> numberOfBins </strong>  :  
is the number of bin the <i>x</i> range will be subdivided into, 
  
<br/><code>argument</code><strong> xMin </strong>  :  
is the lower edge of the histogram,
  
<br/><code>argument</code><strong> xMax </strong>  :  
is the upper edge of the histogram.
  
  

<p/>
For instance
<pre>
   Hist ZpT( "Z0 pT spectrum", 100, 0., 100.);
</pre>
Alternatively you can first declare it and later define it:
<pre>
   Hist ZpT;
   ZpT.book( "Z0 pT spectrum", 100, 0., 100.);
</pre>

Once declared, its contents can be added by repeated calls to fill
<p/><code>method&nbsp; </code><strong> fill( xValue, weight) &nbsp;</strong> <br/>
where
<br/><code>argument</code><strong> xValue </strong>  : 
is the <i>x</i> position where the filling should occur, and
  
<br/><code>argument</code><strong> weight </strong> (<code>default = <strong>1.</strong></code>) : 
is the amount of weight to be added at this <i>x</i> value.
  
  

<p/>
For instance
<pre>
   ZpT.fill( 22.7, 1.); 
</pre>
Since the weight defaults to 1 the last argument could have been 
omitted in this case.   

<p/>
A histogram can be printed by making use of the overloaded &lt;&lt; 
operator, e.g.:
<pre>
   cout &lt;&lt; ZpT;
</pre>

<p/>
A set of overloaded operators have been defined, so that histograms can 
be added, divided by each other (bin by bin) and so on. Also overloaded 
operations with double real numbers are available, so that e.g. 
histograms easily can be rescaled. Thus one may write e.g.
<pre>
  allpT = ZpT + 2. * HpT
</pre>
assuming that <code>allpT</code>, <code>ZpT</code> and <code>HpT</code>
have been booked with the same number of bins and <i>x</i> range. That 
responsibility rests on the user; some checks are made for compatibility, 
but not enough to catch all possible mistakes. 

<p/>
Some further possibilities are included, like writing out histogram 
contents as a table, for plotting e.g. with Gnuplot.

</body>
</html>

<!-- Copyright C 2007 Torbjorn Sjostrand -->
