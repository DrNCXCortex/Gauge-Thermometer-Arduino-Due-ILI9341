# Gauge-Thermometer-Arduino-Due-ILI9341

Graphical Gauge displayed on ILI9341 on Arduino DUE (+VGA support), used thermistor NTC103 (10Kohm at 25°C), used 4color bitmap for gauge background image (for saving memory).

<img src="images/001.jpg" />
<img src="images/002.jpg" />
<img src="images/003.jpg" />
<pre>
/******************************************************************************/
/*                                                                            */
/*  THERMOMETER GAUFE EXAMPLE FOR ARDUINO DUE                                 */
/*                                                                            */
/******************************************************************************/
/*  Copyright (c) 2014  Dr. NCX (mirracle.mxx@gmail.com)                      */
/*                                                                            */
/* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL              */
/* WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED              */
/* WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR    */
/* BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES      */
/* OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,     */
/* WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,     */
/* ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS        */
/* SOFTWARE.                                                                  */
/*                                                                            */
/*  MIT license, all text above must be included in any redistribution.       */
/******************************************************************************/ 


/*----------------------------------------------------------------------------*/
/*  connection of thermistor:                                                 */
/*                                                                            */
/*  GND = --[ 10K ]--  A6 --[ NTC ]-- +3.3V                                   */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/*  ILI9341:                                                                  */
/*----------------------------------------------------------------------------*/
/*   8 = RST                                                                  */
/*   9 = D/C                                                                  */
/*  10 = CS                                                                   */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/*  VGA:                                                                      */
/*----------------------------------------------------------------------------*/
/*  41 = R --[470R]--     }                                                   */
/*  40 = R --[ 1K ]--     } = VGA 1 (RED)                                     */
/*  39 = R --[ 2K2]--     }                                                   */
/*                                                                            */
/*  38 = G --[470R]--     }                                                   */
/*  37 = G --[ 1K ]--     } = VGA 2 (GREEN)                                   */
/*  36 = G --[ 2K2]--     }                                                   */
/*                                                                            */
/*  35 = B --[390R]--     } = VGA 3 (BLUE)                                    */
/*  34 = B --[820R]--     }                                                   */
/*                                                                            */
/*  43 = Hsync --[ 82R]--  = VGA 13                                           */
/*  42 = Vsync --[ 82R]--  = VGA 14                                           */
/*                                                                            */
/*----------------------------------------------------------------------------*/
</pre>
