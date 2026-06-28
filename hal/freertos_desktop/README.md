# FreeRTOS-Kernel für Windows-Simulation

Diese Library benötigt den offiziellen FreeRTOS-Kernel.

## Setup

1. FreeRTOS-Kernel in dieses Verzeichnis clonen:

   ```
   cd lib/freertos_windows
   git clone --branch V11.1.0 --depth 1 https://github.com/FreeRTOS/FreeRTOS-Kernel.git kernel
   ```

2. Danach sollte folgende Struktur existieren:

   ```
   lib/freertos_windows/
     kernel/
       include/         ← FreeRTOS-Header (FreeRTOS.h, task.h, ...)
       portable/
         MSVC-MingW/    ← Windows-Port (port.c, portmacro.h)
         MemMang/       ← Heap-Implementierungen (heap_4.c)
       tasks.c
       queue.c
       list.c
       timers.c
       event_groups.c
       stream_buffer.c
   ```

3. Symlinks/Kopierschritte werden von `setup_freertos.py` automatisiert
   (oder manuell gemäß README).

## Was der Windows-Port liefert

- Preemptives Task-Scheduling via Windows-Threads (CreateThread)
- Tick-Interrupt via Windows-Multimedia-Timer (winmm)
- Benötigt `-lwinmm` beim Linken
