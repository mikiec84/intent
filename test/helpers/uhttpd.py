#!/usr/bin/python
import argparse, BaseHTTPServer, re, sys, time, threading

HOST_NAME = ''
DEFAULT_PORT_NUMBER = 19746
QUIT_URL = '/quit'

HTML_MSG = '<html><body><p>%s</p></body></html>'
SIZE_PAT = re.compile(r'.*?(\d+)\.txt')

quit = False

class MyHandler(BaseHTTPServer.BaseHTTPRequestHandler):
    error_message_format = '<html><head><title>Error</title></head><body><h1>Error %(code)d.</h1><p>%(message)s.</p></body></html>'
    def should_quit(self):
        if self.path == QUIT_URL:
            global quit
            quit = True
            self.send_doc(HTML_MSG % 'Okay, quitting.')
            return True
    def send_doc(self, doc, code=200):
        self.send_response(code)
        self.send_header("Content-Type", "text/html")
        self.end_headers()
        self.wfile.write(doc)
    def do_HEAD(self):
        if self.path.endswith('.html'):
            self.send_response(200)
            self.send_header("Content-Type", "text/html")
            self.end_headers()
        else:
            self.send_error(404)
    def do_GET(self):
        if not self.should_quit():
            if self.path.endswith('.html'):
                self.send_doc(HTML_MSG % self.path)
            else:
                m = SIZE_PAT.match(self.path)
                if m:
                    size = int(m.group(1))
                    if size > 1024 * 1024 * 8:
                        self.send_error(404)
                    else:
                        txt = '0123456789' * 10
                        txt = txt[:-1] + '\n'
                        while len(txt) < size:
                            txt = txt + txt
                        txt = txt[0:size]
                        self.send_response(200)
                        self.send_header("Content-Type", "text/plain")
                        self.end_headers()
                        self.wfile.write(txt)
                else:
                    self.send_error(404)
    def do_POST(self):
        if not self.should_quit():
            length = int(self.headers['Content-Length'])
            data = self.rfile.read(length)
            self.send_doc(HTML_MSG % ('Received %d bytes: "%s"' % (len(data), data)))
    def do_PUT(self):
        self.do_POST()

last_call_lock = None
last_call = None
def server_thread_main():
    global quit
    global last_call
    server_class = BaseHTTPServer.HTTPServer
    httpd = server_class((HOST_NAME, port), MyHandler)
    sys.stderr.write('''Listening on port %s...
  GET /<any path>.html returns simple web pages.
  GET /<any path><number>.txt returns text with size=<number>
  POST /<any url> accepts data and returns html reporting bytes received.
  PUT /<any url> works like POST.
  HEAD /<any path>.html works.
  GET, POST, or PUT /quit makes the server exit.
  Most other requests return 404.
CTRL+C to quit.

''' % port)
    while not quit:
        try:
            httpd.handle_request()
            if last_call_lock:
                last_call_lock.acquire()
            last_call = time.time()
            if last_call_lock:
                last_call_lock.release()
        except KeyboardInterrupt:
            quit = True
    httpd.server_close()

if __name__ == '__main__':
    # Define very simple cmdline syntax, including --help.
    parser = argparse.ArgumentParser(description='Run super-simple http server.', epilog='CTRL+C or GET /quit to stop.')
    parser.add_argument('--logfile', metavar='PATH', help='where to log output, if not stdout')
    parser.add_argument('--timeout', metavar='N', help='exit if no requests in N seconds')
    parser.add_argument('port', help='port on which to listen (default = %s)' % DEFAULT_PORT_NUMBER, nargs='?')
    args = parser.parse_args()
    port = args.port
    if port:
        port = int(port)
    else:
        port = DEFAULT_PORT_NUMBER

    old_stderr = None
    if args.logfile:
        old_stderr = sys.stderr
        sys.stderr = open(args.logfile, 'w')
    try:
        if args.timeout:
            daemon = threading.Thread(target=server_thread_main)
            args.timeout = int(args.timeout)
            daemon.daemon = True
            daemon.start()
            last_call_lock = threading.Lock()
            last_call_lock.acquire()
            last_call = time.time()
            last_call_lock.release()
            while not quit:
                time.sleep(0.5)
                now = time.time()
                last_call_lock.acquire()
                diff = now - last_call
                last_call_lock.release()
                if (diff) >= args.timeout:
                    print('Exiting due to inactivity.')
                    sys.exit(1)
        else:
            server_thread_main()
    finally:
        if old_stderr:
            sys.stderr = old_stderr