/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support 
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include "tc.h"
#include "common.h"
//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------
//*----------------------------------------------------------------------------
//* \fn    AT91F_AIC_EnableIt
//* \brief Enable corresponding IT number
//*----------------------------------------------------------------------------
__inline void AT91F_AIC_EnableIt (
	AT91PS_AIC pAic,      // \arg pointer to the AIC registers
	unsigned int irq_id ) // \arg interrupt number to initialize
{
    //* Enable the interrupt on the interrupt controller
    pAic->AIC_IECR = 0x1 << irq_id ;
}

//------------------------------------------------------------------------------
/// Configures a Timer Counter to operate in the given mode. Timer is stopped
/// after configuration and must be restarted with TC_Start().
/// to obtain the target frequency.
/// \param pTc  Pointer to an AT91S_TC instance.
/// \param mode  Operating mode.
//------------------------------------------------------------------------------
void TC_Configure(AT91S_TC *pTc, unsigned int mode)
{
    // Disable TC clock
    pTc->TC_CCR = AT91C_TC_CLKDIS;

    // Disable interrupts
    pTc->TC_IDR = 0xFFFFFFFF;

    // Clear status register
    pTc->TC_SR;

    // Set mode
    pTc->TC_CMR = mode;

    //compare value
    pTc->TC_RC = 0xFFFF;


    AT91F_AIC_ConfigureIt ( AT91C_ID_TC0, TIMER0_INTERRUPT_LEVEL, AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL, ( void (*)( void ) ) vTMER_0_ISR_Wrapper);
    AT91C_BASE_TC0->TC_IER = AT91C_TC_CPCS;  //  IRQ enable CPC
	AT91F_AIC_EnableIt (AT91C_BASE_AIC, AT91C_ID_TC0);

}

//------------------------------------------------------------------------------
/// Starts the timer clock.
/// \param pTc  Pointer to an AT91S_TC instance.
//------------------------------------------------------------------------------
void TC_Start(AT91S_TC *pTc)
{
    pTc->TC_CCR = AT91C_TC_CLKEN | AT91C_TC_SWTRG;
}

//------------------------------------------------------------------------------
/// Stops the timer clock.
/// \param pTc  Pointer to an AT91S_TC instance.
//------------------------------------------------------------------------------
void TC_Stop(AT91S_TC *pTc)
{
    pTc->TC_CCR = AT91C_TC_CLKDIS;
}

//------------------------------------------------------------------------------
/// Finds the best MCK divisor given the timer frequency and MCK. The result
/// is guaranteed to satisfy the following equation:
///   (MCK / (DIV * 65536)) <= freq <= (MCK / DIV)
/// with DIV being the highest possible value.
/// Returns 1 if a divisor could be found; otherwise returns 0.
/// \param freq  Desired timer frequency.
/// \param mck  Master clock frequency.
/// \param div  Divisor value.
/// \param tcclks  TCCLKS field value for divisor.
//------------------------------------------------------------------------------
unsigned char TC_FindMckDivisor(
    unsigned int freq,
    unsigned int mck,
    unsigned int *div,
    unsigned int *tcclks)
{
    const unsigned int divisors[5] = {2, 8, 32, 128,
#if defined(at91sam9260) || defined(at91sam9261) || defined(at91sam9263) \
    || defined(at91sam9xe) || defined(at91sam9rl64) || defined(at91cap9)
        BOARD_MCK / 32768};
#else
        1024};
#endif
    unsigned int index = 0;
    // Satisfy lower bound
    while (freq < ((mck / divisors[index]) / 65536)) {
        index++;
        // If no divisor can be found, return 0
        if (index == 5) {
            return 0;
        }
    }
    // Try to maximise DIV while satisfying upper bound
    while (index < 4) {
        if (freq > (mck / divisors[index + 1])) {
            break;
        }
        index++;
    }
    // Store results
    if (div) {
        *div = divisors[index];
    }
    if (tcclks) {
        *tcclks = index;
    }
    return 1;
}

void vTimer_0_ISR_Handler(void){
	AT91C_BASE_TC0->TC_SR;
	AT91C_BASE_AIC->AIC_EOICR = 0;
}


//wraper for timer 0 ISR
void vTMER_0_ISR_Wrapper( void ){
	// Save the context of the interrupted task.
	//portSAVE_CONTEXT();

	// Call the handler itself.  This must be a separate function as it uses
	//the stack.
	__asm volatile ("bl vTimer_0_ISR_Handler");
	// Restore the context of the task that is going to
	//execute next. This might not be the same as the originally
	//interrupted task.
	//portRESTORE_CONTEXT();
}

