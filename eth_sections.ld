/* ETH DMA sections — appended to main linker script */
.RxDescripSection : { . = ALIGN(32); *(.RxDescripSection) } >RAM_D2
.TxDescripSection : { . = ALIGN(32); *(.TxDescripSection) } >RAM_D2
.Rx_PoolSection   : { . = ALIGN(32); *(.Rx_PoolSection)   } >RAM_D2
