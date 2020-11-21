from time import sleep
from machine import Pin

import api
from display import display, graphics, menu
import wifi

wifi.do_connect()

# https://micronote.tech/2020/02/Timers-and-Interrupts-with-a-NodeMCU-and-MicroPython/
def debounce(pin):
    prev = None
    for _ in range(32):
        current_value = pin.value()
        if prev != None and prev != current_value:
            return None
        prev = current_value
    return prev

def show_health(health):
    display.fill(0)

    if health.is_healthy():
        display.text('API healthy', 0, 20)
    else:
        display.text('API unhealthy!', 0, 20)

    display.show()

def load_and_show_drinks_menu():
    drinks = api.get_drinks()

    show_drinks_menu(drinks)

def show_drinks_menu(items):
    display.fill(0)

    text = []

    for item in items:
        text.append([item.name, 'scrobble_item(' + str(item.id) + ')'])

    menu.initText(text)

    display.show()

def scrobble_item(item_id):
    display.fill(0)

    display.text('Scrobbling...', 0, 20)

    display.show()

    scrobble = api.post_scrobble(item_id)

    if scrobble == None:
        display.text('Something went wrong', 0, 30)
        display.show()
    else:
        display.text('Done!', 0, 30)
        display.show()

    sleep(3)

    load_and_show_drinks_menu()

# health = api.get_health()
# show_health(health)

# if health.is_healthy():
#     load_and_show_drinks_menu()

button_up = Pin(38, Pin.IN, Pin.PULL_UP)
button_down= Pin(37, Pin.IN, Pin.PULL_UP)
button_select= Pin(36, Pin.IN, Pin.PULL_UP)

def button_up_callback(pin):
    d = debounce(pin)

    if d == None:
        return
    elif d:
        menu.moveUp()
    else:
        return

def button_down_callback(pin):
    d = debounce(pin)

    if d == None:
        return
    elif d:
        menu.moveDown()
    else:
        return

def button_select_callback(pin):
    d = debounce(pin)

    if d == None:
        return
    elif d:
        menu.click()
    else:
        return

button_up.irq(trigger = Pin.IRA_FALLING | Pin.IRQ_RISING, handler = button_up_callback)
button_down.irq(trigger = Pin.IRA_FALLING | Pin.IRQ_RISING, handler = button_down_callback)
button_select.irq(trigger = Pin.IRA_FALLING | Pin.IRQ_RISING, handler = button_select_callback)
