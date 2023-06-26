# The RBS Neuronal-Net


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

The Neuronal Net will consist of 12 Neurons.