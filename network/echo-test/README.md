# echo-test

This application will send 256 individual bytes from 0x00 to 0xFF to an endpoint, and check the
same value is received in the response.

## Running Server

There is a python script in the `echo-server` folder you should run first.

```shell
$ python server.py 
 * Serving Flask app 'server'
 * Debug mode: on
WARNING: This is a development server. Do not use it in a production deployment. Use a production WSGI server instead.
 * Running on all addresses (0.0.0.0)
 * Running on http://127.0.0.1:5000
 * Running on http://192.168.1.100:5000
Press CTRL+C to quit
```

This will listen on port 5000 of the machine it is running on.

## Building

```shell
make release disk
```

After creating the disk, run it to your FujiNet in the usual manner (e.g. copy disk to TNFS server and load it from there
or copy it to the SD and run it from there)

## Running

The application asks for the endpoint to the service that will echo the data back.
Any errors will be reported as the test progresses.
