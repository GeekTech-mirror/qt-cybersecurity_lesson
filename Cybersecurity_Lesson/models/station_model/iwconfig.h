#ifndef IW_H
#define IW_H

#include <QDebug>

#include <stdio.h>
#include <linux/if.h>
#include <linux/string.h>
#include <linux/wireless.h>
#include <sys/ioctl.h>

/* -------------------------- IOCTL LIST -------------------------- */
#define SIOCSIWMODE	0x8B06		/* set operation mode */

#define IW_NUM_OPER_MODE	7
#define IWERR_ARG_TYPE		-3
#define IWERR_SET_EXT		-6

/* Fixup to be able to include kernel includes in userspace.
 * Basically, kill the sparse annotations... Jean II */
#ifndef __user
#define __user
#endif

static int errarg;

/* Modes as human readable strings */
const char * const iw_operation_mode[] = { "Auto",
                    "Ad-Hoc",
                    "Managed",
                    "Master",
                    "Repeater",
                    "Secondary",
                    "Monitor",
                    "Unknown/bug" };

/*------------------------------------------------------------------*/
/*
 * Wrapper to push some Wireless Parameter in the driver
 */
static inline int
iw_set_ext(int			skfd,		/* Socket to the kernel */
       const char *		ifname,		/* Device name */
       int			request,	/* WE ID */
       struct iwreq *	pwrq)		/* Fixed part of the request */
{
  /* Set device name */
  strncpy(pwrq->ifr_name, ifname, IFNAMSIZ);
  /* Do the request */
  return(ioctl(skfd, request, pwrq));
}


static int
set_mode_info(int		skfd,       /* The socket */
          char *		ifname,     /* Dev name */
          char *		arg,		/* Command line args */
          int		count)		/* Args count */
{
  struct iwreq		wrq;
  unsigned int		k;		/* Must be unsigned */

  /* Avoid "Unused parameter" warning */
  count = count;

  /* Check if it is a uint, otherwise get is as a string */
  if(sscanf(arg, "%i", &k) != 1)
    {
      k = 0;
      while((k < IW_NUM_OPER_MODE) &&
        strncasecmp(arg, iw_operation_mode[k], 3))
    k++;
    }
  if(k >= IW_NUM_OPER_MODE)
    {
      errarg = 0;
      return(IWERR_ARG_TYPE);
    }

  wrq.u.mode = k;
  if(iw_set_ext(skfd, ifname, SIOCSIWMODE, &wrq) < 0)
    qDebug() << "failed to set interface in monitor mode - do you have permissions?";
    return(IWERR_SET_EXT);

  /* 1 arg */
  return(1);
}


/*------------------------------------------------------------------*/
/*
 * Open a socket.
 * Depending on the protocol present, open the right socket. The socket
 * will allow us to talk to the driver.
 */
int
iw_sockets_open(void)
{
  static const int families[] = {
    AF_INET, AF_IPX, AF_AX25, AF_APPLETALK
  };
  unsigned int	i;
  int		sock;

  /*
   * Now pick any (exisiting) useful socket family for generic queries
   * Note : don't open all the socket, only returns when one matches,
   * all protocols might not be valid.
   * Workaround by Jim Kaba <jkaba@sarnoff.com>
   * Note : in 99% of the case, we will just open the inet_sock.
   * The remaining 1% case are not fully correct...
   */

  /* Try all families we support */
  for(i = 0; i < sizeof(families)/sizeof(int); ++i)
    {
      /* Try to open the socket, if success returns it */
      sock = socket(families[i], SOCK_DGRAM, 0);
      if(sock >= 0)
        return sock;
  }

  return -1;
}


#endif // IW_H
