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
for your task. One simple derived class (<code>SuppressSmallPT</code>)
comes with the program, mainly as illustration, and the 
<code>main10.cc</code> program provides a complete (toy) example how 
a derived class could be set up and used.

<p/>
There are six sets of routines, that give you different kinds of
freedom. They are, in no particular order:
<br/>(i) Ones that give you access to the event record in between
the process-level and parton-level steps, or in between the 
parton-level and hadron-level ones. You can study the event record
and decide whether to veto this event.
<br/>(ii) Ones that allow you to set a scale at which the combined
parton-level MI+ISR+FSR downwards evolution in <i>pT</i> is
temporarily interrupted, so the event can be studied and either 
vetoed or allowed to continue the evolution.
<br/>(iii) Ones that allow you to to study the event after the first
few ISR/FSR emissions, or first few MI, so the event can be vetoed 
or allowed to continue the evolution.
<br/>(iv) Ones that allow you to study the latest initial- or
final-state emission and veto that emission, without vetoing the
event as a whole.
<br/>(v) Ones that give you access to the properties of the trial 
hard process, so that you can modify the internal Pythia cross section
by your own correction factors.
<br/>(vi) Ones that let you set the scale of shower evolution, 
specifically for matching in resonance decays.
<br/>They are described further in the following numbered subsections. 

<p/> 
All the possibilities above can be combined freely and also be combined
with the standard flags. An event would then survive only if it survived
each of the possible veto methods. There are no hidden interdependencies 
in this game, but of course some combinations may not be particularly 
meaningful. For instance, if you set <code>PartonLevel:all = off</code> 
then the <code>doVetoPT(...)</code> and <code>doVetoPartonLevel(...)</code> 
locations in the code are not even reached, so they would never be called. 

<p/> 
The effect of the vetoes of types (i), (ii) and (iii) can be studied 
in the output of the 
<code><?php $filepath = $_GET["filepath"];
echo "<a href='EventStatistics.php?filepath=".$filepath."' target='page'>";?>Pythia::statistics()</a></code> 
method. The "Selected" column represents the number of events that were 
found acceptable by the internal Pythia machinery, whereas the "Accepted" 
one are the events that also survived the user cuts. The cross section
is based on the latter number, and so is reduced by the amount associated 
by the vetoed events. Also type (v) modifies the cross section, while
types (iv) and (vi) do not.

<h3>The basic components</h3>

For a derived <code>UserHooks</code> class to be called during the 
execution, a pointer to an object of this class should be handed in 
with the 
<br/><code><?php $filepath = $_GET["filepath"];
echo "<a href='ProgramFlow.php?filepath=".$filepath."' target='page'>";?>
Pythia::setUserHooksPtr( UserHooks*)</a></code>
<br/>method. The first step therefore is to construct your own derived 
class, of course. This must contain a constructor and a destructor. The 
<code>initPtr</code> method comes "for free", and is set up without 
any intervention from you.

<a name="method1"></a>
<p/><strong>UserHooks::UserHooks() &nbsp;</strong> <br/>
  
<strong>virtual UserHooks::~UserHooks() &nbsp;</strong> <br/>
The constructor and destructor do not need to do anything.
  

<a name="method2"></a>
<p/><strong>void UserHooks::initPtr( Info* infoPtr, Settings* settingsPtr, ParticleData* particleDataPtr,  Rndm* rndmPtr, BeamParticle* beamAPtr, BeamParticle* beamBPtr, BeamParticle* beamPomAPtr, BeamParticle* beamPomBPtr, CoupSM* coupSMPtr, PartonSystems* partonSystemsPtr, SigmaTotal* sigmaTotPtr) &nbsp;</strong> <br/>
this (non-virtual) method is automatically called during the 
initialization stage to set several useful pointers, and to set up 
the <code>workEvent</code> below. The corresponding objects can 
later be used to extract some useful information.
<br/><?php $filepath = $_GET["filepath"];
echo "<a href='EventInformation.php?filepath=".$filepath."' target='page'>";?>Info</a>:
general event and run information, including some loop counters.
<br/><?php $filepath = $_GET["filepath"];
echo "<a href='SettingsScheme.php?filepath=".$filepath."' target='page'>";?>Settings</a>: 
the settings used to determine the character of the run. 
<br/><?php $filepath = $_GET["filepath"];
echo "<a href='ParticleDataScheme.php?filepath=".$filepath."' target='page'>";?>ParticleData</a>: 
the particle data used in the event record 
(including <code>workEvent</code> below).
<br/><?php $filepath = $_GET["filepath"];
echo "<a href='RandomNumbers.php?filepath=".$filepath."' target='page'>";?>Rndm</a>: the random number 
generator, that you could also use in your code.
<br/><?php $filepath = $_GET["filepath"];
echo "<a href='BeamRemnants.php?filepath=".$filepath."' target='page'>";?>BeamParticle</a>: 
the <code>beamAPtr</code> and <code>beamBPtr</code> beam particles 
contain info on partons extracted from the two incoming beams, 
on the PDFs used, and more. In cases when diffraction is simulated, 
also special Pomeron beams <code>beamPomAPtr</code> and 
<code>beamPomBPtr</code> are introduced, for the Pomerons residing 
inside the respective proton. 
<br/><?php $filepath = $_GET["filepath"];
echo "<a href='StandardModelParameters.php?filepath=".$filepath."' target='page'>";?>CoupSM</a>: 
Standard Model couplings. 
<br/><?php $filepath = $_GET["filepath"];
echo "<a href='AdvancedUsage.php?filepath=".$filepath."' target='page'>";?>PartonSystems</a>: 
the list of partons that belong to each individual subcollision system.   
<br/><?php $filepath = $_GET["filepath"];
echo "<a href='TotalCrossSections.php?filepath=".$filepath."' target='page'>";?>SigmaTotal</a>: 
total/elastic/diffractive cross section parametrizations.
  

<p/> 
Next you overload the desired methods listed in the sections below. 
These often come in pairs or triplets, where the first must return 
true for the last method to be called. This latter method typically 
hands you a reference to the event record, which you then can use to 
decide whether or not to veto. Often the event record can be quite 
lengthy and difficult to overview. The following methods and data member 
can then come in handy. 

<a name="method3"></a>
<p/><strong>void UserHooks::omitResonanceDecays(const Event& process) &nbsp;</strong> <br/>
is a protected method that you can make use of in your own methods to 
extract a simplified list of the hard process, where all resonance decay 
chains are omitted. Intended for the <code>can/doVetoProcessLevel</code> 
routines. Note that the normal process-level generation does include
resonance decays. That is, if a top quark is produced in the hard process, 
then also decays such as <i>t -> b W+, W+ -> u dbar</i> will be generated 
and stored in <code>process</code>. The <code>omitResonanceDecays</code> 
routine will take the input <code>process</code> and copy it to 
<code>workEvent</code> (see below), minus the resonance decay chains. 
All particles produced in the hard process, such as the top, will be 
considered final-state ones, with positive status and no daughters, 
just as it is before resonances are allowed to decay.
<br/>(In the <code>PartonLevel</code> routines, these decay chains will 
initially not be copied from <code>process</code> to <code>event</code>. 
Instead the combined MI, ISR and FSR evolution is done with the top 
above as final particle. Only afterwards will the resonance decay chains 
be copied over, with kinematics changes reflecting those of the top, and 
showers in the decays carried out.)
<br/>
  


<a name="method4"></a>
<p/><strong>void UserHooks::subEvent(const Event& event, bool isHardest = true) &nbsp;</strong> <br/>
is a protected method that you can make use of in your own methods to 
extract a brief list of the current 
partons of interest, with all irrelevant ones omitted. For the default 
<code>isHardest = true</code> only the outgoing partons from the hardest 
interaction (including the partons added to it by ISR and FSR) are 
extracted, as relevant e.g. for <code>doVetoPT( iPos, event)</code> with 
<code>iPos = 0 - 4</code>. With <code>isHardest = false</code> instead 
the outgoing partons of the latest "subprocess" are extracted, as relevant 
when <code>iPos = 5</code>, where it corresponds to the outgoing partons 
in the currently considered decay. The resut is stored in 
<code>workEvent</code> below. The partons are stored in slots 0 through 
<code>workEvent.size() - 1</code>. The <code>mother1()</code> and 
<code>mother2()</code> indices of a particle in this list both return 0. 
The <code>daughter1()</code> and <code>daughter2()</code> indices both 
return the index of the same parton in the original event record 
(<code>event</code>; possibly <code>process</code>), 
so that you can trace the full history, if of interest. 
  

<a name="method5"></a>
<p/><strong>Event UserHooks::workEvent &nbsp;</strong> <br/>
This protected class member contains the outcome of the above
<code>omitResonanceDecays(...)</code> and
<code>subEvent(...)</code> methods. Alternatively you can use it for 
whatever temporary purposes you wish. You are free to use standard 
operations, e.g. to boost the event to its rest frame before analysis, 
or remove particles that should not be analyzed.  
The <code>workEvent</code> can also be sent on to a 
<?php $filepath = $_GET["filepath"];
echo "<a href='EventAnalysis.php?filepath=".$filepath."' target='page'>";?>jet clustering algorithm</a>.

<h3>(i) Interrupt between the main generation levels</h3>

<a name="method6"></a>
<p/><strong>virtual bool UserHooks::canVetoProcessLevel() &nbsp;</strong> <br/>
In the base class this method returns false. If you redefine it
to return true then the method <code>doVetoProcessLevel(...)</code>
will be called immediately after a hard process (and associated
resonance decays) has been selected and stored in the 
<code><?php $filepath = $_GET["filepath"];
echo "<a href='EventRecord.php?filepath=".$filepath."' target='page'>";?>process</a></code> event record.
<br/>At this stage, the <code>process</code> record typically contains 
the two beams in slots 1 and 2, the two incoming partons to the hard 
process in slots 3 and 4, the N (usually 1, 2 or 3) primary produced 
particles in slots 5 through 4 + N, and thereafter recursively the 
resonance decay chains, if any. Use the method 
<code>omitResonanceDecays(...)</code> if you want to skip these 
decay chains. There are exceptions to this structure, 
for <?php $filepath = $_GET["filepath"];
echo "<a href='QCDProcesses.php?filepath=".$filepath."' target='page'>";?>soft QCD processes</a> (where
the partonic process may not yet have been selected at this stage), 
and when <?php $filepath = $_GET["filepath"];
echo "<a href='ASecondHardProcess.php?filepath=".$filepath."' target='page'>";?>a second hard process</a> has
been requested (where two hard processes are bookkept). In general
it is useful to begin the development work by listing a few 
<code>process</code> records, to clarify what the structure is for 
the cases of interest.
  

<a name="method7"></a>
<p/><strong>virtual bool UserHooks::doVetoProcessLevel(Event& process) &nbsp;</strong> <br/>
can optionally be called, as described above. You can study the 
<code>process</code> event record of the hard process. 
Based on that you can decide whether to veto the event, true, or let 
it continue to evolve, false. If you veto, then this event is not
counted among the accepted ones, and does not contribute to the estimated
cross section. The <code>Pytha::next()</code> method will begin a 
completely new event, so the vetoed event will not appear in the 
output of <code>Pythia::next()</code>.
<br/><b>Warning:</b> Normally you should not modify the <code>process</code>
event record. However, for some matrix-element-matching procedures it may 
become unavoidable. If so, be very careful, since there are many pitfalls.
Only to give one example: if you modify the incoming partons then also
the information stored in the beam particles may need to be modified. 
<br/><b>Note:</b> the above veto is different from setting the flag
<code><?php $filepath = $_GET["filepath"];
echo "<a href='MasterSwitches.php?filepath=".$filepath."' target='page'>";?>PartonLevel:all = off</a></code>. 
Also in the latter case the event generation will stop after the process 
level, but an event generated up to this point is considered perfectly 
acceptable. It can be studied and it contributes to the cross section. 
That is, <code>PartonLevel:all = off</code> is intended for simple studies 
of hard processes, where one can save a lot of time by not generating 
the rest of the story. By contrast, the <code>doVetoProcessLevel()</code> 
method allows you to throw away uninteresting events at an early stage 
to save time, but those events that do survive the veto are allowed to 
develop into complete final states (unless flags have been set otherwise). 
  

<a name="method8"></a>
<p/><strong>virtual bool UserHooks::canVetoPartonLevel() &nbsp;</strong> <br/>
In the base class this method returns false. If you redefine it
to return true then the method <code>doVetoPartonLevel(...)</code>
will be called immediately after the parton level has been generated 
and stored in the <code><?php $filepath = $_GET["filepath"];
echo "<a href='EventRecord.php?filepath=".$filepath."' target='page'>";?>event</a></code> 
event record. Thus showers, multiple interactions and beam remnants 
have been set up, but hadronization and decays have not yet been 
performed. This is already a fairly complete event, possibly with quite
a complex parton-level history. Therefore it is usually only meaningful
to study the hardest interaction, e.g. using <code>subEvent(...)</code>
introduced above, or fairly generic properties, such as the parton-level 
jet structure.
  

<a name="method9"></a>
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
<code><?php $filepath = $_GET["filepath"];
echo "<a href='MasterSwitches.php?filepath=".$filepath."' target='page'>";?>HadronLevel:all = off</a></code>. 
Also in the latter case the event generation will stop after the parton 
level, but an event generated up to this point is considered perfectly 
acceptable. It can be studied and it contributes to the cross section.
That is, <code>HadronLevel:all = off</code> is intended for simple 
studies of complete partonic states, where one can save time by not 
generating the complete hadronic final state. By contrast, the 
<code>doVetoPartonLevel()</code> method allows you to throw away 
uninteresting events to save time that way, but those events that 
do survive the veto are allowed to develop into complete final states 
(unless flags have been set otherwise). 
  

<h3>(ii) Interrupt during the parton-level evolution, at a 
<i>pT</i> scale</h3>

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
Recall that the new or modified partons resulting from a MI, ISR or FSR 
step are always appended to the end of the then-current event record.
Previously existing partons are not touched, except for the 
<?php $filepath = $_GET["filepath"];
echo "<a href='ParticleProperties.php?filepath=".$filepath."' target='page'>";?>status, mother and daughter</a> 
values, which are updated to reflect the modified history. It is 
therefore straightforward to find the partons associated with the most 
recent occurence.
<br/>An MI results in four new partons being appended, two incoming
and two outgoing ones.
<br/>An ISR results in the whole affected system being copied down,
with one of the two incoming partons being replaced by a new one, and
one more outgoing parton.
<br/>An FSR results in three new partons, two that come from the
branching and one that takes the recoil.
<br/>The story becomes more messy when rescattering is allowed as part
of the MI machinery. Then there will not only be a new system, as 
outlined above, but additionally some existing systems will undergo
cascade effects, and be copied down with changed kinematics. 

<p/>
In this subsection we outline the possibility to interrupt at a given 
<i>pT</i> scale, in the next to interrupt after a given number of 
emissions.

<a name="method10"></a>
<p/><strong>virtual bool UserHooks::canVetoPT() &nbsp;</strong> <br/>
In the base class this method returns false. If you redefine it
to return true then the method <code>doVetoPT(...)</code> will 
interrupt the downward evolution at <code>scaleVetoPT()</code>. 

<a name="method11"></a>
<p/><strong>virtual double UserHooks::scaleVetoPT() &nbsp;</strong> <br/>
In the base class this method returns 0. You should redefine it
to return the <i>pT</i> scale at which you want to study the event.
  

<a name="method12"></a>
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
for each decaying resonance in a chain such as <i>t -> b W, W -> u dbar</i>.
  
  
<br/><code>argument</code><strong> event </strong>  :  the event record contains a list of all partons 
generated so far, also including intermediate ones not part of the 
"current final state", and also those from further multiple interactions. 
This may not be desirable for comparisons with matrix-element calculations.
You may want to make use of the <code>subEvent(...)</code> method below to 
obtain a simplified event record <code>workEvent</code>. 
  
  

<h3>(iii) Interrupt during the parton-level evolution, after a step</h3>

These options are closely related to the ones above in section (ii), so 
we do not repeat the introduction, nor the possibilities to study the 
event record, also by using <code>subEvent(...)</code> and 
<code>workEvent</code>.  
What is different is that the methods in this section give access to the 
event as it looks like after each of the first few steps in the downwards
evolution, irrespectively of the <i>pT</i> scales of these branchings.
Furthermore, it is here assumed that the focus normally is on the hardest
subprocess, so that ISR/FSR emissions associated with additional MI's
are not considered. For MI studies, however, a separate simpler 
alternative is offered to consider the event after a given number
of interactions.  

<a name="method13"></a>
<p/><strong>virtual bool UserHooks::canVetoStep() &nbsp;</strong> <br/>
In the base class this method returns false. If you redefine it
to return true then the method <code>doVetoStep(...)</code> will 
interrupt the downward ISR and FSR evolution the first
<code>numberVetoStep()</code> times. 

<a name="method14"></a>
<p/><strong>virtual int UserHooks::numberVetoStep() &nbsp;</strong> <br/>
Returns the number of steps <i>n</i> each of ISR and FSR, for the 
hardest interaction, that you want to be able to study. That is,
the method will be called after the first <i>n</i> ISR emissions,
irrespective of the number of FSR ones at the time, and after the 
first <i>n</i> FSR emissions, irespective of the number of ISR ones.
The number of steps defaults to the first one only, but you are free 
to pick another value. Note that double diffraction is handled as two 
separate Pomeron-proton collisions, and thus has two sequences of 
emissions.
  

<a name="method15"></a>
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
  
  

<a name="method16"></a>
<p/><strong>virtual bool UserHooks::canVetoMIStep() &nbsp;</strong> <br/>
In the base class this method returns false. If you redefine it
to return true then the method <code>doVetoMIStep(...)</code> will 
interrupt the downward MI evolution the first
<code>numberVetoMIStep()</code> times. 

<a name="method17"></a>
<p/><strong>virtual int UserHooks::numberVetoMIStep() &nbsp;</strong> <br/>
Returns the number of steps in the MI evolution that you want to be 
able to study, right after each new step has been taken and the 
subcollision has been added to the event record. The number of steps 
defaults to the first one only, but you are free to pick another value.
Note that the hardest interaction of an events counts as the first
multiple interaction. For most hard processes it thus at the first
step offers nothing not available with the <code>VetoProcessLevel</code>
functionality above. For the minimum-bias and diffractive systems the
hardest interaction is not selected at the process level, however, so
there a check after the first multiple interaction offers new 
functionality. Note that double diffraction is handled as two separate
Pomeron-proton collisions, and thus has two sequences of interactions.
Also, if you have set up a second hard process then a check is made
after these first two, and the first interaction coming from the MI
machinery would have sequence number 3.
  

<a name="method18"></a>
<p/><strong>virtual bool UserHooks::doVetoMIStep(int nMI,const Event& event) &nbsp;</strong> <br/>
can optionally be called, as described above. You can study, but not
modify, the <code>event</code> event record of the partonic process. 
Based on that you can decide whether to veto the event, true, or let 
it continue to evolve, false. If you veto, then this event is not
counted among the accepted ones, and does not contribute to the estimated
cross section. The <code>Pytha::next()</code> method will begin a 
completely new event, so the vetoed event will not appear in the 
output of <code>Pythia::next()</code>.
<br/><code>argument</code><strong> nMI </strong>  :  is the number of MI subprocesses has occured
so far.
  
<br/><code>argument</code><strong> event </strong>  :  the event record contains a list of all partons 
generated so far, also including intermediate ones not part of the 
"current final state", e.g. leftovers from the ISR and FSR evolution
of previously generated systems. The most recently added one has not
had time to radiate, of course.
  
  

<h3>(iv) Veto emissions</h3>

The methods in this group are intended to allow the veto of an emission
in ISR or FSR, without affecting the evolution in any other way.
If an emission is vetoed, the event record is "rolled back" to the 
way it was before the emission occured, and the evolution in <i>pT</i> 
is continued downwards from the rejected value. The decision can be 
based on full knowledge of the kinematics of the branching.

<a name="method19"></a>
<p/><strong>virtual bool UserHooks::canVetoISREmission() &nbsp;</strong> <br/>
In the base class this method returns false. If you redefine it
to return true then the method <code>doVetoISREmission(...)</code> 
will interrupt the initial-state shower immediately after each
emission and allow that emission to be vetoed.

<a name="method20"></a>
<p/><strong>virtual bool UserHooks::doVetoISREmission( const int sizeOld, const Event& event) &nbsp;</strong> <br/>
can optionally be called, as described above. You can study, but not
modify, the <code>event</code> event record of the partonic process. 
Based on that you can decide whether to veto the emission, true, or 
not, false. If you veto, then the latest emission is removed from
the event record. In either case the evolution of the shower will
continue from the point where it was left off.
<br/><code>argument</code><strong> sizeOld </strong>  :  is the size of the event record before the
latest emission was added to it. It will also become the new size if 
the emission is vetoed.
  
<br/><code>argument</code><strong> event </strong>  :  the event record contains a list of all partons 
generated so far. Of special interest are the ones associated with the 
most recent emission, which are stored in entries from <code>sizeOld</code> 
through <code>event.size() - 1</code> inclusive. If you veto the emission
these entries will be removed, and the history info in the remaining
partons will be restored to a state as if the emission had never occured.
  
  

<a name="method21"></a>
<p/><strong>virtual bool UserHooks::canVetoFSREmission() &nbsp;</strong> <br/>
In the base class this method returns false. If you redefine it
to return true then the method <code>doVetoFSREmission(...)</code> 
will interrupt the final-state shower immediately after each
emission and allow that emission to be vetoed.

<a name="method22"></a>
<p/><strong>virtual bool UserHooks::doVetoFSREmission( const int sizeOld, const Event& event) &nbsp;</strong> <br/>
can optionally be called, as described above. You can study, but not
modify, the <code>event</code> event record of the partonic process. 
Based on that you can decide whether to veto the emission, true, or 
not, false. If you veto, then the latest emission is removed from
the event record. In either case the evolution of the shower will
continue from the point where it was left off.
<br/><code>argument</code><strong> sizeOld </strong>  :  is the size of the event record before the
latest emission was added to it. It will also become the new size if 
the emission is vetoed.
  
<br/><code>argument</code><strong> event </strong>  :  the event record contains a list of all partons 
generated so far. Of special interest are the ones associated with the 
most recent emission, which are stored in entries from <code>sizeOld</code> 
through <code>event.size() - 1</code> inclusive. If you veto the emission
these entries will be removed, and the history info in the remaining
partons will be restored to a state as if the emission had never occured.
  
  

<h3>(v) Modify cross-sections</h3>

<a name="method23"></a>
<p/><strong>virtual bool UserHooks::canModifySigma() &nbsp;</strong> <br/>
In the base class this method returns false. If you redefine it
to return true then the method <code>multiplySigmaBy(...)</code> will 
allow you to modify the cross section weight assigned to the current
event.
  

<a name="method24"></a>
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

<a name="method25"></a>
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
  
  
 
<h3>(vi) Modify scale in shower evolution</h3>

The choice of maximum shower scale in resonance decays is normally not a
big issue, since the shower here is expected to cover the full phase
space. In some special cases a matching scheme is intended, where hard
radiation is covered by matrix elements, and only softer by showers. The 
below two methods support such an approach. Note that the two methods
are not used in the <code>TimeShower</code> class itself, but when 
showers are called from the <code>PartonLevel</code> generation. Thus
user calls directly to <code>TimeShower</code> are not affected. 

<a name="method26"></a>
<p/><strong>virtual bool UserHooks::canSetResonanceScale() &nbsp;</strong> <br/>
In the base class this method returns false. If you redefine it
to return true then the method <code>scaleResonance(...)</code> 
will set the initial scale of downwards shower evolution.

<a name="method27"></a>
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
  
  

</body>
</html>

<!-- Copyright (C) 2011 Torbjorn Sjostrand -->
