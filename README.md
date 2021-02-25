<h1 align = "center" ># MP3_Player_using_ARM_Cortex_M3</h1>

<h2> Describtion </h2>
this project is an implementation of R2R DAC 
using Arm Cortex m3 cpu "stm32f108c8"
<h2> About DAC </h2>
<h3> DAC Types </h3>
<p> Types of DACs
  <br>
  <b>1. Summing Amplifier</b>
<br>
Since digital to analog conversion is simply a weighted sum of the binary input, a circuit called a summing amplifier is used.
This is basically an op-amp amplifier with multiple resistors connected to one input. The junction where the resistors meet is called the summing junction or the virtual ground. The binary input goes into the resistors and the analog output is obtained on the output of the op-amp.
What makes this circuit work is the resistors – each resistor has to be carefully chosen and matched in order to obtain an accurate analog output. The more bits you have, the more different values of resistors you need – and this is not always practical. The limitations can be overcome by using the next method.
<br>
  <b>2. R-2R Ladder</b>
<br>
This is the simplest type of DAC and needs only two resistor values arranged in a ladder. You can think of this as a somewhat complex voltage divider, though the math is quite complex.
The binary input goes into the 2R resistors and the output is obtained at the bottom of the ladder.
<br>
  <b> 3. PWM DAC </b>
<br>
This is the type of DAC that most of us have used without even knowing it!

The popular Arduino microcontroller has the capability to output analog signals using a PWM signal. On the outset the PWM signal looks like a binary waveform with only high and low peaks with a variable duty cycle (ratio of on time to time period).

However, this is intended to be used with a RC filter to convert the PWM signal into a voltage value by filtering out the AC component and leaving behind the DC component. The voltage output is proportional to the duty cycle of the input – the higher the duty cycle the greater the output voltage of the filter.
</p>
<h2> Static Design </h2>
Component in MCAL Layer 
<br>
GPIO,SYSTIC,RCC,UART
<br>
Component in HAl 
<br>
UART,DAC
<h2> How to Run this Project and Tool </h2>
 <a href ="https://www.st.com/en/development-tools/stm32cubeide.html">STM32Cube </a> 
<h2> Demo </h2>
<a href="https://youtu.be/REQGnVCJNi8">Link_in_YouTube</a>


