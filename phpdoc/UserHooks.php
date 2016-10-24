<html>
<head>
<title>User Hooks</title>
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

<form method='post' action='UserHooks.php'>

<h2>User Hooks</h2>

Sometimes it may be convenient to step in during the generation
process: to modify the built-in cross sections, to veto undesirable
events or simply to collect statistics at various stages of the
evolution. There is a base class <code>UserHooks</code> that gives 
you this access at a few selected places. This class in itself does 
nothing; the idea is that you should write your own derived class
for your task. A few very simple derived classes come with the 
program, mainly as illustration. 

<p/>
For a derived class to be called during the execution, a pointer to
an object of this class should be handed in with the 
<br/>
<?php $filepath = $_GET["filepath"];
echo "<a href='ProgramFlow.php?filepath=".$filepath."' target='page'>";?>
<code>pythia.setUserHooksPtr( UserHooks*)</code></a> method.

<p/>
There are five distinct sets of routines. They are, in no particular 
order: 
<br/>(i) Ones that gives you access to the event record in between
the process-level and parton-level steps, or in between the 
parton-level and hadron-level ones. You can study the event record
and decide whether to veto this event.
<br/>(ii) Ones that allow you to set a scale at with the combined
parton-level MI+ISR+FSR downwards evolution in <i>pT</i> is
temporarily interrupted, so the event can be studied and either 
vetoed or allowed to continue the evolution.
<br/>(iii) Ones that allow you to to study the event after the first
few ISR/FSR emissions, so the event can be vetoed or allowed to 
continue the evolution.
<br/>(iv) Ones that gives you access to the properties of the trial 
hard process, so that you can modify the internal Pythia cross section
by your own correction factors.
<br/>(v) Ones that let you set the scale of shower evolution, 
specifically for matching in resonance decays.
<br/>They are described further in the following. 

<p/>
The first step is to construct your own derived class, of course.

<a name="method1"></a>
<p/><strong>UserHooks::UserHooks() &nbsp;</strong> <br/>
  
<strong>virtual UserHooks::~UserHooks() &nbsp;</strong> <br/>
The constructor and destructor do not need to do anything.
  

<a name="method2"></a>
<p/><strong>void UserHooks::initPtr(PartonSystems* partonSystemsPtr) &nbsp;</strong> <br/>
this (non-virtual) method is called automatically to set a pointer to
the </code>partonSystems</code> object.  
  

<h3>Interrupt between the main generation levels</h3>

<a name="method3"></a>
<p/><strong>virtual bool UserHooks::canVetoProcessLevel() &nbsp;</strong> <br/>
In the base class this method returns false. If you redefine it
to return true then the method <code>doVetoProcessLevel(...)</code>
will be called immediately after a hard process has been selected and
stored in the <code>process</code> event record. 
  

<a name="method4"></a>
<p/><strong>virtual bool UserHooks::doVetoProcessLevel(const Event& process) &nbsp;</strong> <br/>
can optionally be called, as described above. You can study, but not
modify, the <code>process</code> event record of the hard process. 
Based on that you can decide whether to veto the event, true, or let 
it continue to evolve, false. If you veto, then this event is not
counted among the accepted ones, and does not contribute to the estimated
cross section. The <code>Pytha::next()</code> method will begin a 
completely new event, so the vetoed event will not appear in the 
output of <code>Pythia::next()</code>.
<br/><b>Note:</b> the above veto is different from setting the flag
<?php $filepath = $_GET["filepath"];
echo "<a href='MasterSwitches.php?filepath=".$filepath."' target='page'>";?><code>PartonLevel:all = off</code></a>. 
Also in the latter case the event generation will stop after the process 
level, but an event generated up to this point is considered perfectly 
acceptable. It can be studied and cross sections are not affected. 
That is, it is intended for simple studies of hard processes, where one 
can save time by not generating the rest of the story. By contrast, the 
<code>doVetoProcessLevel()</code> method allows you to throw away 
uninteresting events at an early stage to save time that way, but those 
events that do survive the veto are allowed to develop into complete 
final states (unless flags have been set otherwise). 
  

<a name="method5"></a>
<p/><strong>virtual bool UserHooks::canVetoPartonLevel() &nbsp;</strong> <br/>
In the base class this method returns false. If you redefine it
to return true then the method <code>doVetoPartonLevel(...)</code>
will be called immediately after the parton level has been generated 
and stored in the <code>event</code> event record. Thus showers, 
multiple interactions and beam remnants have been set up, but 
hadronization and decays have not yet been performed. 
  

<a name="method6"></a>
<p/><strong>virtual bool UserHooks::doVetoPartonLevel(const Event& event) &nbsp;</strong> <br/>
can optionally be called, as described above. You can study, but not
modify, the <code>event</code> event record of the partonic process. 
Based on that you can decide whether to veto the event, true, or let 
it continue to evolve, false. If you veto, then this event is not
counted among the accepted ones, and does not contribute to the estimated
cross section. The <code>Pytha::next()</code> method will begin a 
completely new event, so the vetoed event will not appear in the 
output of <code>Pythia::next()</code>.
<br/><b>Note:</b> the above veto is different from setting the flag
<?php $filepath = $_GET["filepath"];
echo "<a href='MasterSwitches.php?filepath=".$filepath."' target='page'>";?><code>HadronLevel:all = off</code></a>. 
Also in the latter case the event generation will stop after the parton 
level, but an event generated up to this point is considered perfectly 
acceptable. It can be studied and cross sections are not affected.
That is, it is intended for simple studies of complete partonic states, 
where one can save time by not generating the complete hadronic final 
state. By contrast, the <code>doVetoPartonLevel()</code> method allows 
you to throw away uninteresting events at an early stage to save time 
that way, but those events that do survive the veto are allowed to 
develop into complete final states (unless flags have been set otherwise). 
  

<p/> 
The effect of the vetoes can be studied in the output of the 
<code>Pythia::statistics()</code> method. The "Selected" column represents
the number of events that were found acceptable by the internal Pythia
machinery, whereas the "Accepted" one are the events that also survived 
the user cuts. 

<h3>Interrupt during the parton-level evolution, at a <i>pT</i> scale</h3>

During the parton-level evolution, multiple interactions (MI), 
initial-state radiation (ISR) and final-state radiation (FSR)
are normally evolved downwards in
one interleaved evolution sequence of decreasing <i>pT</i> values.
For some applications, e.g  matrix-element-matching approaches, it
may be convenient to stop the evolution temporarily when the "hard"
emissions have been considered, but before continuing with the more
time-consuming soft activity. Based on these hard partons one can make
a decision whether the event at all falls in the intended event class,
e.g. has the "right" number of parton-level jets. If yes then, as for 
the methods above, the evolution will continue all the way up to a 
complete event. Also as above, if no, then the event will not be 
considered in the final cross section. 

<p/>
In this subsection we outline the possibility to interrupt at a given 
<i>pT</i> scale, in the next to interrupt after a given number of 
emissions.

<a name="method7"></a>
<p/><strong>virtual bool UserHooks::canVetoPT() &nbsp;</strong> <br/>
In the base class this method returns false. If you redefine it
to return true then the method <code>doVetoPT(...)</code> will 
interrupt the downward evolution at <code>scaleVetoPT()</code>. 

<a name="method8"></a>
<p/><strong>virtual double UserHooks::scaleVetoPT() &nbsp;</strong> <br/>
In the base class this method returns 0. You should redefine it
to return the <i>pT</i> scale at which you want to study the event.
  

<a name="method9"></a>
<p/><strong>virtual bool UserHooks::doVetoPT(int iPos, const Event& event) &nbsp;</strong> <br/>
can optionally be called, as described above. You can study, but not
modify, the <code>event</code> event record of the partonic process. 
Based on that you can decide whether to veto the event, true, or let 
it continue to evolve, false. If you veto, then this event is not
counted among the accepted ones, and does not contribute to the estimated
cross section. The <code>Pytha::next()</code> method will begin a 
completely new event, so the vetoed event will not appear in the 
output of <code>Pythia::next()</code>.
<br/><code>argument</code><strong> iPos </strong>  :  is the position/status when the routine is
called, information that can help you decide your course of action: 
<br/><code>argumentoption </code><strong> 0</strong> :  when no MI, ISR or FSR occured above the veto scale;
  
<br/><code>argumentoption </code><strong> 1</strong> :  when inside the interleaved MI + ISR + FSR evolution, 
after an MI process;  
  
<br/><code>argumentoption </code><strong> 2</strong> :  when inside the interleaved MI + ISR + FSR evolution, 
after an ISR emission;
  
<br/><code>argumentoption </code><strong> 3</strong> :  when inside the interleaved MI + ISR + FSR evolution, 
after an FSR emission;
  
<br/><code>argumentoption </code><strong> 4</strong> :  for the optional case where FSR is deferred from the 
interleaved evolution and only considered separately afterward (then 
alternative 3 would never occur);
  
<br/><code>argumentoption </code><strong> 5</strong> :  is for subsequent resonance decays, and is called once
for each decay in a chain such as <i>t -> b W, W -> u dbar</i>.
  
  
<br/><code>argument</code><strong> event </strong>  :  the event record contains a list of all partons 
generated so far, also including intermediate ones not part of the 
"current final state", and also those from further multiple interactions. 
This may not be desirable for comparisons with matrix-element calculations.
You may want to make use of the <code>subEvent(...)</code> method below to 
obtain a simplified event record.
  
  

<a name="method10"></a>
<p/><strong>void UserHooks::subEvent(const Event& event, bool isHardest = true) &nbsp;</strong> <br/>
is a protected method that you can make use of in your own methods 
to extract a brief list of the current partons of interest, with all
irrelevant ones omitted. For the default <code>isHardest = true</code>
only the current partons from the hardest interaction are extracted,
as relevant for <code>doVetoPT( iPos, event)</code> with   
<code>iPos = 0 - 4</code>. With <code>isHardest = false</code> instead 
the latest "subprocess" is extracted, as relevant when 
<code>iPos = 5</code>, where it corresponds to the partons in the 
currently considered decay. The resut is stored in <code>workEvent</code>
below.
  

<a name="method11"></a>
<p/><strong>Event UserHooks::workEvent &nbsp;</strong> <br/>
This protected class member contains the outcome of the above
<code>subEvent(...)</code> method, i.e. a brief list of all current
partons in the hard system considered, with all other partons omitted.
The <code>daughter1()</code> and <code>daughter2()</code> indices of a
particle in this list both return the position of the same parton in the 
original event record (<code>event</code>; possibly <code>process</code>), 
so that you can trace the full history, if of interest. 
The <code>workEvent</code> can e.g. be sent on to a 
<?php $filepath = $_GET["filepath"];
echo "<a href='EventAnalysis.php?filepath=".$filepath."' target='page'>";?>jet clustering algorithm</a>.
You are free to edit  <code>workEvent</code> as you desire, e.g. boost 
to its rest frame before analysis, or remove particles that should 
not be analyzed.

<h3>Interrupt during the parton-level evolution, after a step</h3>

This option is closely related to the one above, so we do not repeat
the introduction, nor the possibilities to study the event record,
also by using <code>subEvent(...)</code> and <code>workEvent</code>.  
What is different is that the methods in this section give access to the 
event as it looks like after each of the first few steps in the downwards
evolution, irrespectively of the <i>pT</i> scales of these branchings.
Furthermore, it is here assumed that the focus is on the hardest
subprocess, so that ISR/FSR emissions associated with additional MI's
are not considered.

<a name="method12"></a>
<p/><strong>virtual bool UserHooks::canVetoStep() &nbsp;</strong> <br/>
In the base class this method returns false. If you redefine it
to return true then the method <code>doVetoStep(...)</code> will 
interrupt the downward ISR and FSR evolution the first
<code>numberVetoStep()</code> times. 

<a name="method13"></a>
<p/><strong>virtual int UserHooks::numberVetoStep() &nbsp;</strong> <br/>
Returns the number of steps each of ISR and FSR, for the hardest
interaction, that you want to be able to study. The number of steps 
defaults to the first one only, but you are freee to pick another value.
  

<a name="method14"></a>
<p/><strong>virtual bool UserHooks::doVetoStep(int iPos, int nISR, int nFSR, const Event& event) &nbsp;</strong> <br/>
can optionally be called, as described above. You can study, but not
modify, the <code>event</code> event record of the partonic process. 
Based on that you can decide whether to veto the event, true, or let 
it continue to evolve, false. If you veto, then this event is not
counted among the accepted ones, and does not contribute to the estimated
cross section. The <code>Pytha::next()</code> method will begin a 
completely new event, so the vetoed event will not appear in the 
output of <code>Pythia::next()</code>.
<br/><code>argument</code><strong> iPos </strong>  :  is the position/status when the routine is
called, information that can help you decide your course of action.
Agrees with options 2 - 5 of the <code>doVetoPT(...)</code> routine
above, while options 0 and 1 are not relevant here.
  
<br/><code>argument</code><strong> nISR </strong>  :  is the number of ISR emissions in the hardest 
process so far. For resonance decays, <code>iPos = 5</code>, it is 0.
  
<br/><code>argument</code><strong> nFSR </strong>  :  is the number of FSR emissions in the hardest 
process so far. For resonance decays, <code>iPos = 5</code>, it is the 
number of emissions in the currently studied system. 
  
<br/><code>argument</code><strong> event </strong>  :  the event record contains a list of all partons 
generated so far, also including intermediate ones not part of the 
"current final state", and also those from further multiple interactions. 
This may not be desirable for comparisons with matrix-element calculations.
You may want to make use of the <code>subEvent(...)</code> method above to 
obtain a simplified event record.
  
  

<h3>Modify cross-sections</h3>

<a name="method15"></a>
<p/><strong>virtual bool UserHooks::canVetoStep() &nbsp;</strong> <br/>
In the base class this method returns false. If you redefine it
to return true then the method <code>multiplySigmaBy(...)</code> will 
allow you to modify the cross section weight assigned to the current
event.
  

<a name="method16"></a>
<p/><strong>virtual double UserHooks::multiplySigmaBy(const SigmaProcess* sigmaProcessPtr, const PhaseSpace* phaseSpacePtr, bool inEvent) &nbsp;</strong> <br/>
when called this method should provide the factor by which you want to 
see the cross section weight of the current event modified by. If you 
return unity then the normal cross section is obtained. Note that, unlike 
the methods above, these modifications do not lead to a difference between
the number of "selected" events and the number of "accepted" ones, 
since the modifications occur already before the "selected" level.
The integrated cross section of a process is modified, of course.
Note that the cross section is only modifiable for normal hard processes.
It does not affect the cross section in further multiple interactions,
nor in elastic/diffractive/minimum-bias events.
<br/><code>argument</code><strong> sigmaProcessPtr, phaseSpacePtr </strong>  : :
what makes this routine somewhat tricky to write is that the 
hard-process event has not yet been constructed, so one is restricted 
to use the information available in the phase-space and cross-section 
objects currently being accessed. Which of their  methods are applicable 
depends on the process, in particular the number of final-state particles. 
The <code>multiplySigmaBy</code> code in <code>UserHooks.cc</code> 
contains explicit instructions about which methods provide meaningful 
information, and so offers a convenient starting point. 
  
<br/><code>argument</code><strong> inEvent </strong>  : : this flag is true when the method is 
called from within the event-generation machinery and false
when it is called at the initialization stage of the run, when the 
cross section is explored to find a maximum for later Monte Carlo usage. 
Cross-section modifications should be independent of this flag,
for consistency, but if <code> multiplySigmaBy(...)</code> is used to
collect statistics on the original kinematics distributions before cuts,
then it is important to be able to exclude the initialization stage
from comparisons.
  
  

<p/>
One derived class is supplied as an example how this facility can be used
to reweight cross sections in the same spirit as is done with QCD cross
sections for the minimum-bias/underlying-event description:
<p/><code>class&nbsp; </code><strong> SuppressSmallPT : public UserHooks &nbsp;</strong> <br/>
suppress small-<i>pT</i> production for <i>2 -> 2</i> processes
only, while leaving other processes unaffected. The basic suppression
factor is <i>pT^4 / ((k*pT0)^2 + pT^2)^2</i>, where <i>pT</i>
refers to the current hard subprocess and <i>pT0</i> is the same
energy-dependent dampening scale as used for 
<?php $filepath = $_GET["filepath"];
echo "<a href='MultipleInteractions.php?filepath=".$filepath."' target='page'>";?>multiple interactions</a>.
This class contains <code>canModifySigma()</code> and
<code>multiplySigmaBy()</code> methods that overload the base class ones.

<a name="method17"></a>
<p/><strong>SuppressSmallPT::SuppressSmallPT( double pT0timesMI = 1., int numberAlphaS = 0, bool useSameAlphaSasMI = true) &nbsp;</strong> <br/>
 The optional arguments of the constructor provides further variability. 
<br/><code>argument</code><strong> pT0timesMI </strong>  :  
corresponds to the additional factor <i>k</i> in the above formula. 
It is by default equal to 1 but can be used to explore deviations from 
the expected value.
  
<br/><code>argument</code><strong> numberAlphaS </strong>  :   
if this number <i>n</i> is bigger than the default 0, the 
corresponding number of <i>alpha_strong</i> factors is also 
reweighted from the normal renormalization scale to a modified one,
i.e. a further suppression factor
<i>( alpha_s((k*pT0)^2 + Q^2_ren) / alpha_s(Q^2_ren) )^n</i>
is introduced.
  
<br/><code>argument</code><strong> useSameAlphaSasMI </strong>  :  
regulates which kind of new <i>alpha_strong</i> value is evaluated
for the numerator in the above expression. It is by default the same 
as set for multiple interactions (i.e. same starting value at 
<i>M_Z</i> and same order of running), but if <code>false</code> 
instead the one for hard subprocesses. The denominator 
<i>alpha_s(Q^2_ren)</i> is always the value used for the "original", 
unweighted cross section. 
  
  
 
<h3>Modify scale in shower evolution</h3>

The choice of maximum shower scale in resonance decays is normally not a
big issue, since the shower here is expected to cover the full phase
space. In some special cases a matching scheme is intended, where hard
radiation is covered by matrix elements, and only softer by showers. The 
below two methods support such an approach. Note that the two methods
are not used in the <code>TimeShower</code> class itself, but when 
showers are called from the <code>PartonLevel</code> generation. Thus
user calls directly to <code>TimeShower</code> are not affected. 

<a name="method18"></a>
<p/><strong>virtual bool UserHooks::canSetResonanceScale() &nbsp;</strong> <br/>
In the base class this method returns false. If you redefine it
to return true then the method <code>scaleResonance(...)</code> 
will set the initial scale of downwards shower evolution.

<a name="method19"></a>
<p/><strong>virtual double UserHooks::scaleResonance( const int iRes, const Event& event) &nbsp;</strong> <br/>
can optionally be called, as described above. You should return the maximum
scale, in GeV, from which the shower evolution will begin. The base class
method returns 0, i.e. gives no shower evolution at all.
You can study, but not modify, the <code>event</code> event record 
of the partonic process to check which resonance is decaying, and into what. 
<br/><code>argument</code><strong> iRes </strong>  :  is the location in the event record of the 
resonance that decayed to the particles that now will shower.
  
<br/><code>argument</code><strong> event </strong>  :  the event record contains a list of all partons 
generated so far, specifically the decaying resonance and its immediate
decay products. 
  
  
 
<h3>Final comments</h3>

All the possibilities above can be combined freely and also be combined
with the standard flags. An event would then survive only if it survived
each of the possible veto methods. There are no hidden interdependencies 
in this game, but of course some combinations may not be particularly 
meaningful. For instance, if you set <code>PartonLevel:all = off</code> 
then the <code>doVetoPT(...)</code> and <code>doVetoPartonLevel(...)</code> 
locations in the code are not even reached, so they would never be called. 

<p/>
An example how the above methods can be used for toy studies is found in 
<code>main10.cc</code>.

</body>
</html>

<!-- Copyright (C) 2009 Torbjorn Sjostrand -->
