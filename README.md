# Mailbox IOT

## What is Mailbox IOT

Mailbox IOT is complete system composed of a micro computer, a mailbox and a web app that send you notification through email when you receive a real letter or package.

## How it works

The onboard system is composed with a micro computer connected to a red LED and a photoresistor. When letters are pushed into the mailbox, they blocks the light thus the processor can handled the difference between initial light signal receive by the photoresistor. When the signal change an email is sent to notifiy the client that is mailbox is full.

![Schema](https://github.com/EZ-Team/Mailbox_IOT/blob/master/resources/schema.jpg)

To detect if the content is a letter or a package, a countdown is trigger to check, after an amount of time, if the photoresistor signal is back to it's initial state. If it's the case then it's a letter, else a package.