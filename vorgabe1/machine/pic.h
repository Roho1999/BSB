/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                  P I C                                    */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Programmable Interrupt Controller.                                        */
/* Mit Hilfe des PICs koennen Hardware-Unterbrechungen einzeln zugelassen    */
/* oder unterdrueckt werden. Auf diese Weise wird also bestimmt, ob die      */
/* Unterbrechung eines Geraetes ueberhaupt an den Prozessor weitergegeben    */
/* wird. Selbst dann erfolgt eine Aktivierung der Unterbrechungsroutine nur, */
/* wenn der Prozessor bereit ist, auf Unterbrechungen zu reagieren. Dies     */
/* kann mit Hilfe der Klasse CPU festgelegt werden.                          */
/*****************************************************************************/

#ifndef __pic_include__
#define __pic_include__

class PIC
 {
private:
    PIC(const PIC &other) = delete; // Verhindere Kopieren
    PIC& operator=(const PIC& other) = delete; // Verhindere Kopieren
public:
    PIC() {}
    /* TODO: Hier muesst ihr selbst Code vervollstaendigen */

    // Lässt zu, dass der PIC Unterbrechungen des Geräts Nummer interrupt_device an den Prozessor weiterleitet.
    void allow (int interrupt_device);
        
    // Verhindert, dass der PIC Unterbrechungen des Geräts Nummer interrupt_device an den Prozessor weiterleitet.
    void forbid (int interrupt_device);

    // Gibt den Zustand des Maskierbits von interrupt_device zurück.
    // Wenn Unterbrechungen dieses Geräts nicht an die CPU weitergeleitet werden, soll true zurückgegeben werden, andernfalls false. 
    bool is_masked (int interrupt_device);  
 };

#endif
