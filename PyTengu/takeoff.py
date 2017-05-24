#!/usr/bin/python3

import asyncio
import asyncio_redis
import time
from xplane.plugin import XPlane

xplane = XPlane()
if __name__ == '__main__':
    loop = asyncio.get_event_loop()
    loop.run_until_complete( xplane.run() )

    # except KeyboardInterrupt:
    #     pass
    # except Exception as e:
    #     print("Got an error: " + str(e) )

# while True:
#     message_generator = xplane.next_message()
#     message = next( message_generator )
#     if message:
#         print( message )

