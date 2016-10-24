<html>
<head>
<title>Implement New Showers</title>
<link rel="stylesheet" type="text/css" href="pythia.css"/>
<link rel="shortcut icon" href="pythia32.gif"/>
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

<form method='post' action='ImplementNewShowers.php'>

<h2>Implement New Showers</h2>

In case you want to replace the PYTHIA initial- and final-state 
showers by your own, it is possible but not trivial. The point is 
that multiple interactions (MI), initial-state radiation (ISR) and
final-state radiation (FSR) in general appear in one single 
interleaved sequence of decreasing <i>pT</i> values. Therefore
shower replacements would have to be able to play the game by such
rules, as we will outline further below. Of course, this still
leaves the field open exactly how to define what to mean by 
<i>pT</i>, how to handle recoil effects, how the colour flow is
affected, and so on, so there is certainly room for alternative 
showers. 

<p/>
For the moment we assume you want to keep the MI part of the story
unchanged, and make use of the existing beam-remnants (BR) machinery. 
If you want to replace both MI, ISR, FSR and BR then you had better 
replace the whole <code>PartonLevel</code> module of the code. 
If, in addition, you want to produce your own hard processes, 
then you only need the
<?php $filepath = $_GET["filepath"];
echo "<a href='HadronLevelStandalone.php?filepath=".$filepath."' target='page'>";?>hadron-level standalone</a>
part of the machinery. 

<p/>
In order to write replacement codes for ISR and/or FSR it is useful
to be aware of which information has to be shared between the 
different components, and which input/output structure is required
of the relevant methods. For details, nothing beats studying the
existing code. However, here we provide an overview, that should
serve as a useful introduction.

<p/>
It should be noted that we here primarily address the problem in 
its full generality, with interleaved MI, ISR and FSR. There exists
an option <code>TimeShower:interleave = off</code> where only
MI and ISR would be interleaved and FSR be considered after these
two, but still before BR. Most of the aspects described here would 
apply also for that case. By contrast, resonance decays are only
considered after all the four above components, and timelike 
showers in those decays would never be interleaved with anything
else, so are much simpler to administrate.

<p/>
Therefore the <?php $filepath = $_GET["filepath"];
echo "<a href='ProgramFlow.php?filepath=".$filepath."' target='page'>";?>
<code>pythia.setShowerPtr( timesDecPtr, timesPtr, spacePtr)</code></a>
method allows two separate pointers to be set to instances of
derived <code>TimeShower</code> classes. The first is only required 
to handle decays, say of <i>Z^0</i> or <i>Upsilon</i>, with no
dependence on beam remnants or ISR. The second, as well as 
<code>spacePtr</code>, has to handle the interleaved evolution of MI, 
ISR and FSR. Therefore you are free to implement only the first, and 
let the PYTHIA default showers take care of the latter two. But, if 
you wanted to, you could also set <code>timesDecPtr = 0</code> and 
only provide a <code>timesPtr</code>, or only a <code>spacePtr</code>.
If your timelike shower does both cases, the first two pointers
can agree. The only tiny point to take into account then is that
<code>init( beamAPtr, beamBPtr)</code> is called twice, a first time 
to <code>timesDecPtr</code> with beam pointers 0, and a second time
to <code>timesPtr</code> with nonvanishing beam pointers.  

<h3>The event record and associated information</h3>  

Obviously the main place for sharing information is the event
record, specifically the <code>Event event</code> member of 
<code>Pythia</code>, passed around as a reference. It is 
assumed you already studied how it works, so here we only draw 
attention to a few aspects of special relevance.

<p/>
One basic principle is that existing partons should not be 
overwritten. Instead new partons should be created, even when a 
parton only receives a slightly shifted momentum and for the rest 
stays the same. Such "carbon copies" by final-state branchings
should be denoted by both daughter indices of the original parton 
pointing to the copy, and both mother indices of the copy to the 
original. If the copy instead is intended to represent an earlier 
step, e.g. in ISR backwards evolution, the role of mothers and 
daughters is interchanged. The 
<code>event.copy( iCopy, newStatus)</code>
routine can take care of this tedious task; the sign of 
<code>newStatus</code> tells the program which case to assume.

<p/>
To make the event record legible it is essential that the 
<?php $filepath = $_GET["filepath"];
echo "<a href='ParticleProperties.php?filepath=".$filepath."' target='page'>";?>status codes</a> 
are selected appropriately to represent the reason why each new 
parton is added to the record. Also remember to change the 
status of a parton to be negative whenever an existing parton
is replaced by a set of new daughter partons.

<p/>
Another important parton property is <code>scale()</code>,
which does not appear in the normal event listing, but only 
if you use the extended
<code>Event:listScaleAndVertex = on</code> option. 
This property is supposed to represent the production scale 
(in GeV) of a parton. In the current FSR and ISR algorithms 
it is used to restrict from above the allowed <i>pT</i> 
values for branchings of this particular parton.  
Beam remnants and other partons that should not radiate are 
assigned scale 0.   

<p/>
Auxiliary to the event record proper is the 
<?php $filepath = $_GET["filepath"];
echo "<a href='AdvancedUsage.php?filepath=".$filepath."' target='page'>";?><code>PartonSystems</code></a>  
class, that keep track of which partons belong together in the 
same scattering subsystem. This information must be kept up-to-date 
during the shower evolution.

<p/>
For initial-state showers it is also necessary to keep track of
the partonic content extracted from the beams. This information
is stored in the 
<?php $filepath = $_GET["filepath"];
echo "<a href='AdvancedUsage.php?filepath=".$filepath."' target='page'>";?><code>BeamParticle</code></a>  
class.  

<h3>The TimeShower interface</h3>

If you want to replace the <code>TimeShower</code> class this would
involve replacing the following methods.

<p/><code>method&nbsp; </code><strong> TimeShower() &nbsp;</strong> <br/>
The constructor does not need to do anything.
  

<p/><code>method&nbsp; </code><strong> void init( BeamParticle* beamAPtrIn = 0, BeamParticle* beamBPtrIn = 0) &nbsp;</strong> <br/>
You have to store your local copy of the pointers to these objects,
since they have to be used during the generation, as explained above.
The pointers could be zero; e.g. a local copy of <code>TimeShower</code>
is created to handle showers in decays such as <i>Upsilon -> q qbar</i>
from inside the <code>ParticleDecays class</code>. This is also the 
place to do initialization of whatever parameters you plan to use, 
e.g. by reading in them from a user-accessible database like the
<code>Settings</code> one.
  

<p/><code>method&nbsp; </code><strong> double enhancePTmax() &nbsp;</strong> <br/>
Relative to the default <i>pT_max</i> evolution scale of the process,
it may still be convenient to vary the matching slightly for the hardest
interaction in an event, to probe the sensitivity to such details. The 
base-class implementation returns the value of the 
<code>TimeShower:pTmaxFudge</code> parameter.
  

<p/><code>method&nbsp; </code><strong> int shower( int iBeg, int iEnd, Event& event, double pTmax) &nbsp;</strong> <br/>
This is an all-in-one call for shower evolution, and as such cannot be 
used for the normal interleaved evolution, where only the routines below
are used. It also cannot be used in resonance decays that form part of
the hard process, since there the 
<?php $filepath = $_GET["filepath"];
echo "<a href='UserHooks.php?filepath=".$filepath."' target='page'>";?>user hooks</a> insert a potential 
veto step. Currently this routine is therefore only used in the
hadron-level decays, e.g. <i>Upsilon -> g g g</i>.
<br/><code>iBeg</code> and <code>iEnd</code> is the position of the
first and last parton of a separate system, typically produced by a 
resonance decay. Such a system only evolves in isolation, and in
particular does not relate to the beams. 
<br/>The <code>pTmax</code> value sets the maximum scale for evolution,
but normally you would restrict that further for each individual
parton based on its respective scale value. 
<br/>The routine is expected to return the number of FSR branchings that 
were generated, but only for non-critical statistics purposes.
<br/>Since the real action typically is delegated to the routines 
below, it may well be that the existing code need not be replaced.
  

<p/><code>method&nbsp; </code><strong> void prepare( int iSys, Event& event) &nbsp;</strong> <br/>
This method is called immediately after a new interaction (or the
products of a resonance decay) has been added, and should then be used 
to prepare the subsystem of partons for subsequent evolution. In
the current code this involves identifying all colour and charge 
dipole ends: the position of radiating and recoiling partons, maximum 
<i>pT</i> scales, possible higher-order matrix elements matchings 
to apply, and so on. 
<br/>The <code>iSys</code> parameter specifies which parton system
is to be prepared. It is used to extract the set of partons to be
treated, with rules as described in the above section on subsystems.  
Specifically, the first two partons represent the incoming state,
or are 0 for resonance decays unrelated to the beams, while the 
rest are not required to be in any particular order.
  

<p/><code>method&nbsp; </code><strong> void update( int iSys, Event& event) &nbsp;</strong> <br/>
This method is called immediately after a spacelike branching in the 
<code>iSys</code>'th subsystem. Thus the information for that system is 
out-of-date, while that of the others is unchanged. If you want, you are 
free to throw away all information for the affected subsystem and call 
<code>prepare( iSys, event)</code> to create new one. Alternatively 
you may choose only to update the information that has changed.
  

<p/><code>method&nbsp; </code><strong> double pTnext( Event& event, double pTbegAll, double pTendAll) &nbsp;</strong> <br/>
This is the main driver routine for the downwards evolution. A new 
<i>pT</i> is to be selected based on the current information set up 
by the routines above, and along with that a branching parton or dipole.
The <code>pTbegAll</code> scale is the maximum scale allowed, from which
the downwards evolution should be begun (usually respecting the maximum 
scale of each individual parton). If no emission is found above 
<code>pTendAll</code> (and above the respective shower cutoff scales) 
then <code>0.</code> should be returned and no emissions will be allowed.
Both scales can vary from one event to the next: if a scale has
already been selected for MI or ISR it makes no sense to look for
a scale smaller than that from FSR, since it would not be able to 
compete, so <code>pTendAll</code> is set correspondingly. As it happens, 
FSR is tried before ISR and MI in the interleaved evolution,
but this is an implementational detail that could well change.  
<br/>Typically the implementation of this routine would be to set
up a loop over all possible radiating objects (dipoles, dipole ends, ...),
for each pick its possible branching scale and then pick the one 
with largest scale as possible winner. At this stage no branching should
actually be carried out, since MI, ISR and FSR still have to be compared
to assign the winner.
  

<p/><code>method&nbsp; </code><strong> bool branch( Event& event) &nbsp;</strong> <br/>
This method will be called once FSR has won the competition with 
MI and ISR to do the next branching. The candidate branching found 
in the previous step should here be carried out in full. The 
pre-branching partons should get a negative status code and new
replacement ones added to the end of the event record. Also the  
subsystem information should be updated, and possibly also the
beams. 
<br/>Should some problem be encountered in this procedure, e.g. if 
some not-previously-considered kinematics requirement fails, it is 
allowed to return <code>false</code> to indicate that no branching 
could be carried out.   
  

<p/><code>method&nbsp; </code><strong> int system() &nbsp;</strong> <br/>
This method is not virtual. If a branching is constructed by the 
previous routine this tiny method should be able to return the number 
of the selected subsystem <code>iSysSel</code> where it occured, 
so that the spacelike shower can be told which system to update, 
if necessary. Therefore <code>iSysSel</code> must be set in 
<code>branch</code> (or already in <code>pTnext</code>).  
  

<p/><code>method&nbsp; </code><strong> void list( ostream& os = cout) &nbsp;</strong> <br/>
This method is not at all required. In the current implementation it 
outputs a list of all the dipole ends, with information on the
respective dipole. The routine is not called anywhere in the public
code, but has been inserted at various places during the 
development/debug phase.
  
   
<h3>The SpaceShower interface</h3>

If you want to replace the <code>SpaceShower</code> class this would
involve replacing the following methods. You will find that much of
the story reminds of <code>TimeShower</code> above, and actually some 
cut-and-paste of text is involved. In some respects the description
is simpler, since there are no special cases for resonance decays
and non-interleaved evolution. Thus there is no correspondence to the 
<code>TimeShower::shower(...)</code> routine.  

<p/><code>method&nbsp; </code><strong> SpaceShower() &nbsp;</strong> <br/>
The constructor does not need to do anything.
  

<p/><code>method&nbsp; </code><strong> void init( BeamParticle* beamAPtrIn, BeamParticle* beamBPtrIn) &nbsp;</strong> <br/>
You have to store your local copy of the pointers to these objects,
since they have to be used during the generation, as explained above.
This is also the place to do initialization of whatever parameters 
you plan to use, e.g. by reading in them from a user-accessible 
database like the <code>Settings</code> one.
  

<p/><code>method&nbsp; </code><strong> bool limitPTmax( Event& event, double Q2Fac = 0., double Q2Ren = 0.) &nbsp;</strong> <br/>
The question is whether the ISR should be allowed to occur at larger
scales than the hard process it surrounds. This is process-dependent.
For instance, if the hard process is <i>Z^0</i> production we know
that ISR should be allowed to go right up to the kinematical limit.
If it is a <i>2 -> 2</i> QCD process the ISR should not exceed
the scale of the hard process, since if so one would doublecount.
The <code>SpaceShower:pTmaxMatch</code> switch allows you to force the
behaviour, or else to program your own logic. The current implementation
limits <i>pT</i> whenever the final state contains a quark (except top),
gluon or photon, since then the danger of doublecounting is there.
You may replace by your own logic, or leave as is. 
<br/>The internal PYTHIA implementation also allows intermediate options, 
where emissions can go up to the kinematical limit but be dampened above 
the factorization or renormalization scale. Therefore the (square of the) 
latter two are provided as optional input parameters.  
  

<p/><code>method&nbsp; </code><strong> double enhancePTmax() &nbsp;</strong> <br/>
When the above method limits <i>pT_max</i> to the scale of the process,
it may still be convenient to vary the matching slightly for the hardest
interaction in an event, to probe the sensitivity to such details. The 
base-class implementation returns the value of the 
<code>SpaceShower:pTmaxFudge</code> parameter.
  

<p/><code>method&nbsp; </code><strong> void prepare( int iSys, Event& event, bool limitPTmax = true) &nbsp;</strong> <br/>
This method is called immediately after a new interaction has been 
added, and should then be used to prepare the subsystem of partons
for subsequent evolution. In the current code this involves identifying 
the colour and charge dipole ends: the position of radiating and recoiling 
partons, maximum <i>pT</i> scales, and possible higher-order matrix 
elements matchings to apply. Depending on what you have in mind you may 
choose to store slightly different quantities. You have to use the 
subsystem information described above to find the positions of the two 
incoming partons (and the outgoing ones) of the system, and from there 
the scales at which they were produced.
<br/> The <code>limitPTmax</code> input agrees with the output of the
previous method for the hardest process, and is always true for
subsequent MI, since there an unlimited <i>pT</i> for sure
would lead to doublecounting.
  

<p/><code>method&nbsp; </code><strong> void update( int iSys, Event& event) &nbsp;</strong> <br/>
This method is called immediately after a timelike branching in the 
<code>iSys</code>'th subsystem. Thus the information for that system may 
be out-of-date, and to be updated. For the standard PYTHIA showers 
this routine does not need to do anything, but that may be different
in another implementation.
  

<p/><code>method&nbsp; </code><strong> double pTnext(( Event& event, double pTbegAll, double pTendAll, int nRadIn = -1) &nbsp;</strong> <br/>
This is the main driver routine for the downwards evolution. A new 
<i>pT</i> is to be selected based on the current information set up 
by the routines above, and along with that a branching parton or dipole.
The <code>pTbegAll</code> scale is the maximum scale allowed, from which
the downwards evolution should be begun (usually respecting the maximum 
scale of each individual parton). If no emission is found above 
<code>pTendAll</code> (and above the respective shower cutoff scales) 
then <code>0.</code> should be returned and no emissions will be allowed.
Both scales can vary from one event to the next: if a scale has
already been selected for MI or ISR it makes no sense to look for
a scale smaller than that from FSR, since it would not be able to 
compete, so <code>pTendAll</code> is set correspondingly. As it happens, 
FSR is tried before ISR and MI in the interleaved evolution,
but this is an implementational detail that could well change.  
<br/>Typically the implementation of this routine would be to set
up a loop over all possible radiating objects (dipoles, dipole ends, ...),
for each pick its possible branching scale and then pick the one 
with largest scale as possible winner. At this stage no branching should
actually be carried out, since MI, ISR and FSR still have to be compared
to assign the winner.
<br/>The final input <code>nRadIn</code> provides the total number of 
ISR and FSR emissions already generated in the event, and so allows a 
special treatment for the very first emission, if desired.
  

<p/><code>method&nbsp; </code><strong> bool branch( Event& event) &nbsp;</strong> <br/>
This method will be called once FSR has won the competition with 
MI and ISR to do the next branching. The candidate branching found 
in the previous step should here be carried out in full. The 
pre-branching partons should get a negative status code and new
replacement ones added to the end of the event record. Also the  
subsystem information should be updated, and possibly also the
beams. 
<br/>Should some problem be encountered in this procedure, e.g. if 
some not-previously-considered kinematics requirement fails, it is 
allowed to return <code>false</code> to indicate that no branching 
could be carried out.   
  

<p/><code>method&nbsp; </code><strong> int system() &nbsp;</strong> <br/>
This method is not virtual. If a branching is constructed by the 
previous routine this tiny method should be able to return the number 
of the selected subsystem <code>iSysSel</code> where it occured, 
so that the spacelike shower can be told which system to update, 
if necessary. Therefore <code>iSysSel</code> must be set in 
<code>branch</code> (or already in <code>pTnext</code>).  
  

<p/><code>method&nbsp; </code><strong> void list( ostream& os = cout) &nbsp;</strong> <br/>
This method is not at all required. In the current implementation it 
outputs a list of all the dipole ends, with information on the
respective dipole. The routine is not called anywhere in the public
code, but has been inserted at various places during the 
development/debug phase.
  
   
</body>
</html>

<!-- Copyright (C) 2008 Torbjorn Sjostrand -->
