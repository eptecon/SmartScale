#ifndef UART_ASYNC_H
#define UART_ASYNC_H

#define TXD_PIN (GPIO_NUM_4)
#define RXD_PIN (GPIO_NUM_5)


void init_uart();

void tx_uart_task(void *param);
void rx_uart_task(void *param);



#endif /* UART_ASYNC */
