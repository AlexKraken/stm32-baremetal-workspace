# Notes

## Overview of Setup
Configure GPIOA/ADC12 pin
- Enable clock access to GPIOA
- Set PA0 to analog mode

Configure ADC peripheral
- Enable clock access to ADC
- Set ADC common clock mode
- Set ADC to single/continuous conversion mode
- Enable ADC internal voltage regulator

- *The above registers cannot be set unless ADC is disabled*

Enable and configure ADC module
- Set ADC enable control
- Wait for startup time of voltage regulator
- Set conversion sequence ranking
- Set conversion sequence length

### Configuring ADC Sequence
*Ex: ADC1 configured with 3 channels: Ch2, Ch4, Ch5*

If the desired conversion sequence is:
1. Ch5
2. Ch2
3. Ch4

Then the sequence ranking:

    ADC1_SQR1_SQ1[4:0] = 0b0101 // 5
    ADC1_SQR1_SQ2[4:0] = 0b0010 // 2
    ADC1_SQR1_SQ3[4:0] = 0b0100 // 4

And the sequence length:

    ADC1_SQR1_L[3:0] = 0b011  // 3

With the resulting C code:

    // Ch5: 0b0101
    ADC1 -> SQR1 |= ADC_SQR1_SQ1_2;
    ADC1 -> SQR1 |= ADC_SQR1_SQ1_0;
    // Ch2: 0b0010
    ADC1 -> SQR1 |= ADC_SQR1_SQ2_1;
    // Ch4: 0b0100
    ADC1 -> SQR1 |= ADC_SQR1_SQ3_2;

    // Length 3: 0b011
    ADC1 -> SQR1 |= ADC_SQR1_L_1;
    ADC1 -> SQR1 |= ADC_SQR1_L_0;
