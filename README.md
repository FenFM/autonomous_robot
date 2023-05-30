<div align = "center">
  <h1> Autonomous Robot Project </h1>
  <h4> A project for our University about an autonomous robot/car </h4>
&nbsp;

![Repository Size][rep-size-img] &nbsp; ![Code Size][code-size-img] &nbsp; ![Code File Size][code-file-img] &nbsp; ![Lines of Code][code-lines-img]
&nbsp;

<h2> Concept of the Neuronal Net of the Robot </h2>

``` mermaid
    flowchart TD
        IR_1[IR 1] --> n_00(N 00)
        IR_2[IR 2] --> n_01(N 01)
        IR_3[IR 3] --> n_02(N 02)
        IR_4[IR 4] --> n_03(N 03)
        IR_5[IR 5] --> n_04(N 04)
        IR_6[IR 6] --> n_05(N 05)

        n_10(N 10)
        n_11(N 11)
        n_12(N 12)
        n_13(N 13)
        n_20(N 20)
        n_21(N 21)

        n_00 --> n_10
        n_01 --> n_10
        n_02 --> n_10

        n_00 --> n_11
        n_01 --> n_11
        n_02 --> n_11

        n_03 --> n_12
        n_04 --> n_12
        n_05 --> n_12

        n_03 --> n_13
        n_04 --> n_13
        n_05 --> n_13

        n_10 --> n_20
        n_12 --> n_20

        n_11 --> n_21
        n_13 --> n_21

        n_20 --> m_l[Motor Left]
        n_21 --> m_r[Motor Right]
```
</div>

$$ \begin{align*}
\text{IR} \,\, &\dots \,\, \text{Infrared Sensor} \\
\text{N}  \,\, &\dots \,\, \text{Neuron}
\end{align*} $$


[code-size-img]:    https://img.shields.io/github/languages/code-size/FenFr/autonomous_robot?label=Code%20Size&style=flat-square
[code-file-img]:    https://img.shields.io/github/directory-file-count/fenfr/autonomous_robot?label=Files&style=flat-square
[code-lines-img]:   https://img.shields.io/tokei/lines/github/fenfr/autonomous_robot?label=Lines%20of%20Code&style=flat-square
[rep-size-img]:     https://img.shields.io/github/repo-size/FenFr/autonomous_robot?label=Repo%20Size&style=flat-square
