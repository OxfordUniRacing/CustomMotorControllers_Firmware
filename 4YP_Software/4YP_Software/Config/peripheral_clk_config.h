/* Auto-generated config file peripheral_clk_config.h */
#ifndef PERIPHERAL_CLK_CONFIG_H
#define PERIPHERAL_CLK_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <h> AFEC Clock Settings
// <y> AFEC Clock source
// <CONF_SRC_MCK"> Master Clock (MCK)
// <i> This defines the clock source for the AFEC
// <id> afec_clock_source
#ifndef CONF_AFEC0_SRC
#define CONF_AFEC0_SRC CONF_SRC_MCK
#endif
// </h>

/**
 * \def AFEC FREQUENCY
 * \brief AFEC's Clock frequency
 */
#ifndef CONF_AFEC0_FREQUENCY
#define CONF_AFEC0_FREQUENCY 150000000
#endif

// <h> AFEC Clock Settings
// <y> AFEC Clock source
// <CONF_SRC_MCK"> Master Clock (MCK)
// <i> This defines the clock source for the AFEC
// <id> afec_clock_source
#ifndef CONF_AFEC1_SRC
#define CONF_AFEC1_SRC CONF_SRC_MCK
#endif
// </h>

/**
 * \def AFEC FREQUENCY
 * \brief AFEC's Clock frequency
 */
#ifndef CONF_AFEC1_FREQUENCY
#define CONF_AFEC1_FREQUENCY 150000000
#endif

/**
 * \def CONF_HCLK_FREQUENCY
 * \brief HCLK's Clock frequency
 */
#ifndef CONF_HCLK_FREQUENCY
#define CONF_HCLK_FREQUENCY 300000000
#endif

/**
 * \def CONF_FCLK_FREQUENCY
 * \brief FCLK's Clock frequency
 */
#ifndef CONF_FCLK_FREQUENCY
#define CONF_FCLK_FREQUENCY 300000000
#endif

/**
 * \def CONF_CPU_FREQUENCY
 * \brief CPU's Clock frequency
 */
#ifndef CONF_CPU_FREQUENCY
#define CONF_CPU_FREQUENCY 300000000
#endif

/**
 * \def CONF_SLCK_FREQUENCY
 * \brief Slow Clock frequency
 */
#define CONF_SLCK_FREQUENCY 32000

/**
 * \def CONF_MCK_FREQUENCY
 * \brief Master Clock frequency
 */
#define CONF_MCK_FREQUENCY 150000000

/**
 * \def CONF_PCK6_FREQUENCY
 * \brief Programmable Clock Controller 6 frequency
 */
#define CONF_PCK6_FREQUENCY 12000000

// <h> PWM Clock Settings
// <y> PWM Clock source
// <CONF_SRC_MCK"> Master Clock (MCK)
// <i> This defines the clock source for the PWM
// <id> pwm_clock_source
#ifndef CONF_PWM0_SRC
#define CONF_PWM0_SRC CONF_SRC_MCK
#endif
// </h>

/**
 * \def PWM FREQUENCY
 * \brief PWM's Clock frequency
 */
#ifndef CONF_PWM0_FREQUENCY
#define CONF_PWM0_FREQUENCY 150000000
#endif

// <h> PWM Clock Settings
// <y> PWM Clock source
// <CONF_SRC_MCK"> Master Clock (MCK)
// <i> This defines the clock source for the PWM
// <id> pwm_clock_source
#ifndef CONF_PWM1_SRC
#define CONF_PWM1_SRC CONF_SRC_MCK
#endif
// </h>

/**
 * \def PWM FREQUENCY
 * \brief PWM's Clock frequency
 */
#ifndef CONF_PWM1_FREQUENCY
#define CONF_PWM1_FREQUENCY 150000000
#endif

// <y> TC Clock Source
// <CONF_SRC_MCK"> Master Clock (MCK)
// <id> tc_clock_source
// <i> Select the clock source for TC.
#ifndef CONF_TC0_SRC
#define CONF_TC0_SRC CONF_SRC_MCK
#endif

/**
 * \def CONF_TC0_FREQUENCY
 * \brief TC0's Clock frequency
 */
#ifndef CONF_TC0_FREQUENCY
#define CONF_TC0_FREQUENCY 150000000
#endif

// <y> TC Clock Source
// <CONF_SRC_MCK"> Master Clock (MCK)
// <id> tc_clock_source
// <i> Select the clock source for TC.
#ifndef CONF_TC3_SRC
#define CONF_TC3_SRC CONF_SRC_MCK
#endif

/**
 * \def CONF_TC3_FREQUENCY
 * \brief TC3's Clock frequency
 */
#ifndef CONF_TC3_FREQUENCY
#define CONF_TC3_FREQUENCY 150000000
#endif

// <h> USART Clock Settings
// <o> USART Clock source

// <0=> Master Clock (MCK)
// <1=> MCK / 8 for USART
// <2=> Programmable Clock Controller 4 (PMC_PCK4)
// <3=> External Clock
// <i> This defines the clock source for the USART
// <id> usart_clock_source
#ifndef CONF_USART1_CK_SRC
#define CONF_USART1_CK_SRC 0
#endif

// <o> USART External Clock Input on SCK <1-4294967295>
// <i> Inputs the external clock frequency on SCK
// <id> usart_clock_freq
#ifndef CONF_USART1_SCK_FREQ
#define CONF_USART1_SCK_FREQ 10000000
#endif

// </h>

/**
 * \def USART FREQUENCY
 * \brief USART's Clock frequency
 */
#ifndef CONF_USART1_FREQUENCY
#define CONF_USART1_FREQUENCY 150000000
#endif

// <h> MCAN Clock Settings
// <y> MCAN Clock source
// <CONF_SRC_PCK5"> Programmable Clock Controller 5 (PMC_PCK5)
// <i> This defines the clock source for the MCAN
// <id> mcan_clock_source
#ifndef CONF_MCAN1_SRC
#define CONF_MCAN1_SRC CONF_SRC_PCK5
#endif
// </h>

/**
 * \def MCAN FREQUENCY
 * \brief MCAN's Clock frequency
 */
#ifndef CONF_MCAN1_FREQUENCY
#define CONF_MCAN1_FREQUENCY 12000000
#endif

// <<< end of configuration section >>>

#endif // PERIPHERAL_CLK_CONFIG_H
