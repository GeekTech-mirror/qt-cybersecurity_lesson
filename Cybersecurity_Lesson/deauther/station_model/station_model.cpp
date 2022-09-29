#include "station_model.h"
#include "station_model_p.h"

#include "custom_colors.h"

StationModel::StationModel(QObject *parent)
    : QAbstractItemModel{parent}
{

}

/* Tree Model
** --------
** reimplement functions from Tree Model
** --------
**/

/* Return data from model
** Parameters:
**     index: specify model index for row and column
**     role: controls the display method for the model contents
** Return: value stored in the model index
*/
QVariant StationModel::data (const QModelIndex &index, int role) const
{
    // Checks for valid index
    if (!index.isValid())
        return QVariant();

    //qDebug() << role;

    switch (role) {
//    case Qt::BackgroundRole:
//        if (0 == index.row() % 2)
//            return CustomColors::frame_color();
//        else
//            return CustomColors::frame_color().lighter(145);

//        break;
    case Qt::ForegroundRole:
        return CustomColors::frame_font_color();
        break;
    case Qt::DecorationRole:
        // Display Symbol in first column
        if (index.column() == 0)
        {
            return QIcon ();
        }
        break;
    case Qt::DisplayRole:
        return QVariant();
        break;
    case Qt::EditRole:
        return QVariant();
        break;
    }

    return QVariant();
}


/* Store value in model index */
bool StationModel::setData (const QModelIndex &index,
                            const QVariant &network_role,
                            int role)
{
    if (role != Qt::EditRole)
        return false;

    StationItem *item = d_ptr->getItem (index, this);

    // Set WiFi info to display
    bool result = d_ptr->setItemRole (network_role, index, this);
    if (result)
        Q_EMIT dataChanged (index, index, item->changedRoles());
        item->clearChangedRoles();

    return result;
}


/* Return header data from model
** Parameters:
**     index: specify model index for row and column
**     orientation: control location of header (vertical or horizontal)
**     role: controls the display method for the model contents
** Return: value stored in the model index
*/
QVariant StationModel::headerData (int section,
                                   Qt::Orientation orientation,
                                   int role) const
{
    switch (role) {
    case Qt::ForegroundRole:
        return CustomColors::frame_font_color();
    case Qt::DisplayRole:
        return rootItem->data (section);
        break;
    }
    return QVariant();
}

/* Store header data in model index */
bool StationModel::setHeaderData (int section,
                                  Qt::Orientation orientation,
                                  const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    const bool result = rootItem->setHeaderData (section, value.toString());

    if (result)
        emit headerDataChanged (orientation, section, section);

    return result;
}


/* Return index to model */
QModelIndex StationModel::index (int row, int column,
                                  const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    StationItem *parentItem = d_ptr->getItem (parent, this);

    if (!parentItem)
        return QModelIndex();

    StationItem *childItem = parentItem->child (row);
    if (childItem)
        return createIndex (row, column, childItem);

    return QModelIndex();
}


/* Return index to parent in model */
QModelIndex StationModel::parent (const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    StationItem *childItem = static_cast<StationItem*>(index.internalPointer());
    StationItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}


/* Return total number of rows */
int StationModel::rowCount (const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() > 0)
        return 0;

    const StationItem *parentItem = d_ptr->getItem (parent, this);

    return parentItem ? parentItem->childCount() : 0;
}


/* Insert number of rows below specified position */
bool StationModel::insertRows (int position, int rows,
                                const QModelIndex &parent)
{
    StationItem *parentItem = d_ptr->getItem (parent, this);
    if (!parentItem)
        return false;

    beginInsertRows (parent, position, position + rows - 1);
    const bool success = parentItem->insertChildren(position,
                                                    rows,
                                                    rootItem->columnCount());
    endInsertRows ();

    return success;
}

/* Remove number of rows below specified position */
bool StationModel::removeRows (int position, int rows,
                                const QModelIndex &parent)
{
    StationItem *parentItem = d_ptr->getItem (parent, this);
    if (!parentItem)
        return false;

    beginRemoveRows (parent, position, position + rows - 1);
    const bool success = parentItem->removeChildren (position, rows);
    endRemoveRows ();

    return success;
}


/* Return total number of columns */
int StationModel::columnCount (const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return rootItem->columnCount();
}


/* Returns list of item flags present on index (See Qt::ItemFlags) */
Qt::ItemFlags StationModel::flags (const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags (index);
}


/* Station Model
** --------
** Implement functions to add stations to the model
** --------
**/
void StationModel::addStation ()
{

}

void StationModel::capturePacket ()
{
    int n;

    /* fill oui struct if ram is greater than 32 MB */
    if (get_ram_size() > MIN_RAM_SIZE_LOAD_OUI_RAM)
    {
        lopt.manufList = load_oui_file();
    }


    // while (1)
    if (lopt.do_exit)
    {
        break;
    }

    if (time(NULL) - tt1 >= lopt.file_write_interval)
    {
        /* update the text output files */

        tt1 = time(NULL);
        if (opt.output_format_csv)
            dump_write_csv(lopt.ap_1st, lopt.st_1st, lopt.f_encrypt);
        if (opt.output_format_kismet_csv)
            dump_write_kismet_csv(lopt.ap_1st, lopt.st_1st, lopt.f_encrypt);
        if (opt.output_format_kismet_netxml)
            dump_write_kismet_netxml(lopt.ap_1st,
                                     lopt.st_1st,
                                     lopt.f_encrypt,
                                     lopt.airodump_start_time);
    }

    /* flush the output files */
    if (opt.f_cap != NULL) fflush(opt.f_cap);
    if (opt.f_ivs != NULL) fflush(opt.f_ivs);

    gettimeofday(&tv1, NULL);

    if (lopt.active_scan_sim > 0
        && cycle_time2 > lopt.active_scan_sim * 1000)
    {
        gettimeofday(&tv4, NULL);
        send_probe_requests(wi, lopt.num_cards);
    }

    if (cycle_time > 500000)
    {
        gettimeofday(&tv3, NULL);
        update_rx_quality();
        if (lopt.s_iface != NULL)
        {
            check_monitor(wi, fd_raw, &fdh, lopt.num_cards);
            if (lopt.singlechan) check_channel(wi, lopt.num_cards);
            if (lopt.singlefreq) check_frequency(wi, lopt.num_cards);
        }
    }

    if (opt.s_file != NULL)
    {
        static struct timeval prev_tv = {0, 0};

        /* Read one packet */
        n = sizeof(pkh);

        if (fread(&pkh, (size_t) n, 1, lopt.f_cap_in) != 1)
        {
            memset(lopt.message, '\x00', sizeof(lopt.message));
            snprintf(lopt.message,
                     sizeof(lopt.message),
                     "][ Finished reading input file %s.",
                     opt.s_file);
            opt.s_file = NULL;
            continue;
        }

        if (lopt.pfh_in.magic == TCPDUMP_CIGAM)
        {
            SWAP32(pkh.caplen);
            SWAP32(pkh.len);
        }

        n = caplen = pkh.caplen;

        memset(buffer, 0, sizeof(buffer));
        h80211 = buffer;

        if (n <= 0 || n > (int) sizeof(buffer))
        {
            memset(lopt.message, '\x00', sizeof(lopt.message));
            snprintf(lopt.message,
                     sizeof(lopt.message),
                     "][ Finished reading input file %s.",
                     opt.s_file);
            opt.s_file = NULL;
            continue;
        }

        if (fread(h80211, (size_t) n, 1, lopt.f_cap_in) != 1)
        {
            memset(lopt.message, '\x00', sizeof(lopt.message));
            snprintf(lopt.message,
                     sizeof(lopt.message),
                     "][ Finished reading input file %s.",
                     opt.s_file);
            opt.s_file = NULL;
            continue;
        }

        if (lopt.pfh_in.linktype == LINKTYPE_PRISM_HEADER)
        {
            if (h80211[7] == 0x40)
            {
                n = 64;
                ri.ri_power = -((int32_t) load32_le(h80211 + 0x33));
                ri.ri_noise = (int32_t) load32_le(h80211 + 0x33 + 12);
                ri.ri_rate = load32_le(h80211 + 0x33 + 24) * 500000;
            }
            else
            {
                n = load32_le(h80211 + 4);
                ri.ri_mactime = load64_le(h80211 + 0x5C - 48);
                ri.ri_channel = load32_le(h80211 + 0x5C - 36);
                ri.ri_power = -((int32_t) load32_le(h80211 + 0x5C));
                ri.ri_noise = (int32_t) load32_le(h80211 + 0x5C + 12);
                ri.ri_rate = load32_le(h80211 + 0x5C + 24) * 500000;
            }

            if (n < 8 || n >= caplen) continue;

            memcpy(tmpbuf, h80211, (size_t) caplen);
            caplen -= n;
            memcpy(h80211, tmpbuf + n, (size_t) caplen);
        }

        if (lopt.pfh_in.linktype == LINKTYPE_RADIOTAP_HDR)
        {
            /* remove the radiotap header */

            n = load16_le(h80211 + 2);

            if (n <= 0 || n >= caplen) continue;

            int got_signal = 0;
            int got_noise = 0;
            struct ieee80211_radiotap_iterator iterator;
            struct ieee80211_radiotap_header * rthdr;

            rthdr = (struct ieee80211_radiotap_header *) h80211;

            if (ieee80211_radiotap_iterator_init(
                    &iterator, rthdr, caplen, NULL)
                < 0)
                continue;

            /* go through the radiotap arguments we have been given
             * by the driver
             */

            while (ieee80211_radiotap_iterator_next(&iterator) >= 0)
            {
                switch (iterator.this_arg_index)
                {
                    case IEEE80211_RADIOTAP_TSFT:
                        ri.ri_mactime = le64_to_cpu(
                            *((uint64_t *) iterator.this_arg));
                        break;

                    case IEEE80211_RADIOTAP_DBM_ANTSIGNAL:
                    case IEEE80211_RADIOTAP_DB_ANTSIGNAL:
                        if (!got_signal)
                        {
                            if (*iterator.this_arg < 127)
                                ri.ri_power = *iterator.this_arg;
                            else
                                ri.ri_power = *iterator.this_arg - 255;

                            got_signal = 1;
                        }
                        break;

                    case IEEE80211_RADIOTAP_DBM_ANTNOISE:
                    case IEEE80211_RADIOTAP_DB_ANTNOISE:
                        if (!got_noise)
                        {
                            if (*iterator.this_arg < 127)
                                ri.ri_noise = *iterator.this_arg;
                            else
                                ri.ri_noise = *iterator.this_arg - 255;

                            got_noise = 1;
                        }
                        break;

                    case IEEE80211_RADIOTAP_ANTENNA:
                        ri.ri_antenna = *iterator.this_arg;
                        break;

                    case IEEE80211_RADIOTAP_CHANNEL:
                        ri.ri_channel = getChannelFromFrequency(
                            le16toh(*(uint16_t *) iterator.this_arg));
                        break;

                    case IEEE80211_RADIOTAP_RATE:
                        ri.ri_rate = (*iterator.this_arg) * 500000;
                        break;
                }
            }

            memcpy(tmpbuf, h80211, (size_t) caplen);
            caplen -= n;
            memcpy(h80211, tmpbuf + n, (size_t) caplen);
        }

        if (lopt.pfh_in.linktype == LINKTYPE_PPI_HDR)
        {
            /* remove the PPI header */

            n = load16_le(h80211 + 2);

            if (n <= 0 || n >= caplen) continue;

            /* for a while Kismet logged broken PPI headers */
            if (n == 24 && load16_le(h80211 + 8) == 2) n = 32;

            if (n <= 0 || n >= caplen) continue; //-V560

            memcpy(tmpbuf, h80211, (size_t) caplen);
            caplen -= n;
            memcpy(h80211, tmpbuf + n, (size_t) caplen);
        }

        read_pkts++;

        if (lopt.relative_time && prev_tv.tv_sec != 0
            && prev_tv.tv_usec != 0)
        {
            // handle delaying this packet
            struct timeval pkt_tv;
            pkt_tv.tv_sec = pkh.tv_sec;
            pkt_tv.tv_usec = pkh.tv_usec;

            const useconds_t usec_diff
                = (useconds_t) time_diff(&prev_tv, &pkt_tv);

            if (usec_diff > 0) usleep(usec_diff);
        }
        else if (read_pkts % 10 == 0)
            usleep(1);

        // track the packet's timestamp
        prev_tv.tv_sec = pkh.tv_sec;
        prev_tv.tv_usec = pkh.tv_usec;
    }
    else if (lopt.s_iface != NULL)
    {
        /* capture one packet */

        FD_ZERO(&rfds);
        for (i = 0; i < lopt.num_cards; i++)
        {
            FD_SET(fd_raw[i], &rfds); // NOLINT(hicpp-signed-bitwise)
        }

        tv0.tv_sec = lopt.update_s;
        tv0.tv_usec = (lopt.update_s == 0) ? REFRESH_RATE : 0;

        gettimeofday(&tv1, NULL);

        if (select(fdh + 1, &rfds, NULL, NULL, &tv0) < 0)
        {
            if (errno == EINTR)
            {
                gettimeofday(&tv2, NULL);

                time_slept += 1000000UL * (tv2.tv_sec - tv1.tv_sec)
                              + (tv2.tv_usec - tv1.tv_usec);

                continue;
            }
            perror("select failed");

            /* Restore terminal */
            show_cursor();

            return (EXIT_FAILURE);
        }
    }
    else
        usleep(1);

    gettimeofday(&tv2, NULL);

    time_slept += 1000000UL * (tv2.tv_sec - tv1.tv_sec)
                  + (tv2.tv_usec - tv1.tv_usec);

    if (time_slept > REFRESH_RATE && time_slept > lopt.update_s * 1000000)
    {
        time_slept = 0;

        update_dataps();

        /* update the window size */

        if (ioctl(0, TIOCGWINSZ, &(lopt.ws)) < 0)
        {
            lopt.ws.ws_row = 25;
            lopt.ws.ws_col = 80;
        }

        /* display the list of access points we have */

        if (!lopt.do_pause && !lopt.background_mode)
        {
            ALLEGE(pthread_mutex_lock(&(lopt.mx_print)) == 0);

            dump_print(lopt.ws.ws_row, lopt.ws.ws_col, lopt.num_cards);

            ALLEGE(pthread_mutex_unlock(&(lopt.mx_print)) == 0);
        }
        continue;
    }

    if (opt.s_file == NULL && lopt.s_iface != NULL)
    {
        for (i = 0; i < lopt.num_cards; i++)
        {
            if (FD_ISSET(fd_raw[i], &rfds)) // NOLINT(hicpp-signed-bitwise)
            {

                memset(buffer, 0, sizeof(buffer));
                h80211 = buffer;
                if ((caplen = wi_read(
                         wi[i], NULL, NULL, h80211, sizeof(buffer), &ri))
                    == -1)
                {
                    wi_read_failed++;
                    if (wi_read_failed > 1)
                    {
                        lopt.do_exit = 1;
                        break;
                    }
                    memset(lopt.message, '\x00', sizeof(lopt.message));
                    snprintf(lopt.message,
                             sizeof(lopt.message),
                             "][ interface %s down ",
                             wi_get_ifname(wi[i]));

                    // reopen in monitor mode
                    strlcpy(ifnam, wi_get_ifname(wi[i]), sizeof(ifnam));

                    wi_close(wi[i]);
                    wi[i] = wi_open(ifnam);
                    if (!wi[i])
                    {
                        printf("Can't reopen %s\n", ifnam);
￼﻿
                        /* Restore terminal */
                        show_cursor();

                        exit(EXIT_FAILURE);
                    }

                    fd_raw[i] = wi_fd(wi[i]);
                    if (fd_raw[i] > fdh) fdh = fd_raw[i];

                    break;
                }

                read_pkts++;

                wi_read_failed = 0;
                dump_add_packet(h80211, caplen, &ri, i);
            }
        }
    }
    else if (opt.s_file != NULL)
    {
        dump_add_packet(h80211, caplen, &ri, i);
    }
}

void StationModel::write_packet ()
{
    if (ap_cur != NULL)
    {
        if (h80211[0] == 0x80 && lopt.one_beacon)
        {
            if (!ap_cur->beacon_logged)
                ap_cur->beacon_logged = 1;
            else
                return (0);
        }
    }

    if (opt.record_data)
    {
        if (((h80211[0] & 0x0C) == 0x00) && ((h80211[0] & 0xF0) == 0xB0))
        {
            /* authentication packet */
            check_shared_key(h80211, (size_t) caplen);
        }
    }

    if (ap_cur != NULL)
    {
        if (ap_cur->security != 0 && lopt.f_encrypt != 0
            && ((ap_cur->security & lopt.f_encrypt) == 0))
        {
            return (1);
        }

        if (is_filtered_essid(ap_cur->essid))
        {
            return (1);
        }
    }

    /* this changes the local ap_cur, st_cur and na_cur variables and should be
     * the last check before the actual write */
    if (caplen < 24 && caplen >= 10 && h80211[0])
    {
        /* RTS || CTS || ACK || CF-END || CF-END&CF-ACK*/
        //(h80211[0] == 0xB4 || h80211[0] == 0xC4 || h80211[0] == 0xD4 ||
        // h80211[0] == 0xE4 || h80211[0] == 0xF4)

        /* use general control frame detection, as the structure is always the
         * same: mac(s) starting at [4] */
        if (h80211[0] & 0x04)
        {
            p = h80211 + 4;
            while ((uintptr_t) p <= adds_uptr((uintptr_t) h80211, 16)
                   && (uintptr_t) p <= adds_uptr((uintptr_t) h80211, caplen))
            {
                memcpy(namac, p, 6);

                if (memcmp(namac, NULL_MAC, 6) == 0)
                {
                    p += 6;
                    continue;
                }

                if (memcmp(namac, BROADCAST, 6) == 0)
                {
                    p += 6;
                    continue;
                }

                if (lopt.hide_known)
                {
                    /* check AP list */
                    ap_cur = lopt.ap_1st;

                    while (ap_cur != NULL)
                    {
                        if (!memcmp(ap_cur->bssid, namac, 6)) break;

                        ap_cur = ap_cur->next;
                    }

                    /* if it's an AP, try next mac */

                    if (ap_cur != NULL)
                    {
                        p += 6;
                        continue;
                    }

                    /* check ST list */
                    st_cur = lopt.st_1st;

                    while (st_cur != NULL)
                    {
                        if (!memcmp(st_cur->stmac, namac, 6)) break;

                        st_cur = st_cur->next;
                    }

                    /* if it's a client, try next mac */

                    if (st_cur != NULL)
                    {
                        p += 6;
                        continue;
                    }
                }

                /* not found in either AP list or ST list, look through NA list
                 */
                na_cur = lopt.na_1st;
                na_prv = NULL;

                while (na_cur != NULL)
                {
                    if (!memcmp(na_cur->namac, namac, 6)) break;

                    na_prv = na_cur;
                    na_cur = na_cur->next;
                }

                /* update our chained list of unknown stations */
                /* if it's a new mac, add it */

                if (na_cur == NULL)
                {
                    if (!(na_cur
                          = (struct NA_info *) malloc(sizeof(struct NA_info))))
                    {
                        perror("malloc failed");
                        return (1);
                    }

                    memset(na_cur, 0, sizeof(struct NA_info));

                    if (lopt.na_1st == NULL)
                        lopt.na_1st = na_cur;
                    else
                        na_prv->next = na_cur;

                    memcpy(na_cur->namac, namac, 6);

                    na_cur->prev = na_prv;

                    gettimeofday(&(na_cur->tv), NULL);
                    na_cur->tinit = time(NULL);
                    na_cur->tlast = time(NULL);

                    na_cur->power = -1;
                    na_cur->channel = -1;
                    na_cur->ack = 0;
                    na_cur->ack_old = 0;
                    na_cur->ackps = 0;
                    na_cur->cts = 0;
                    na_cur->rts_r = 0;
                    na_cur->rts_t = 0;
                }

                /* update the last time seen & power*/

                na_cur->tlast = time(NULL);
                na_cur->power = ri->ri_power;
                na_cur->channel = ri->ri_channel;

                switch (h80211[0] & 0xF0)
                {
                    case 0xB0:
                        if (p == h80211 + 4) na_cur->rts_r++;
                        if (p == h80211 + 10) na_cur->rts_t++;
                        break;

                    case 0xC0:
                        na_cur->cts++;
                        break;

                    case 0xD0:
                        na_cur->ack++;
                        break;

                    default:
                        na_cur->other++;
                        break;
                }

                /*grab next mac (for rts frames)*/
                p += 6;
            }
        }
    }

    if (opt.f_cap != NULL && caplen >= 10)
    {
        pkh.len = pkh.caplen = (uint32_t) caplen;

        gettimeofday(&tv, NULL);

        pkh.tv_sec = (int32_t) tv.tv_sec;
        pkh.tv_usec = (int32_t) tv.tv_usec;

        n = sizeof(pkh);

        if (fwrite(&pkh, 1, n, opt.f_cap) != (size_t) n)
        {
            perror("fwrite(packet header) failed");
            return (1);
        }

        fflush(stdout);

        n = pkh.caplen;

        if (fwrite(h80211, 1, n, opt.f_cap) != (size_t) n)
        {
            perror("fwrite(packet data) failed");
            return (1);
        }

        fflush(stdout);
    }
}

static struct oui * load_oui_file(void)
{
    FILE * fp;
    char * manuf;
    char buffer[BUFSIZ];
    unsigned char a[2];
    unsigned char b[2];
    unsigned char c[2];
    struct oui *oui_ptr = NULL, *oui_head = NULL;

    fp = open_oui_file();
    if (!fp)
    {
        return (NULL);
    }

    memset(buffer, 0x00, sizeof(buffer));
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        if (!(strstr(buffer, "(hex)"))) continue;

        memset(a, 0x00, sizeof(a));
        memset(b, 0x00, sizeof(b));
        memset(c, 0x00, sizeof(c));
        // Remove leading/trailing whitespaces.
        trim(buffer);
        if (sscanf(buffer, "%2c-%2c-%2c", (char *) a, (char *) b, (char *) c)
            == 3)
        {
            if (oui_ptr == NULL)
            {
                if (!(oui_ptr = (struct oui *) malloc(sizeof(struct oui))))
                {
                    fclose(fp);
                    perror("malloc failed");
                    return (NULL);
                }
            }
            else
            {
                if (!(oui_ptr->next
                      = (struct oui *) malloc(sizeof(struct oui))))
                {
                    fclose(fp);
                    perror("malloc failed");

                    while (oui_head != NULL)
                    {
                        oui_ptr = oui_head->next;
                        free(oui_head);
                        oui_head = oui_ptr;
                    }
                    return (NULL);
                }
                oui_ptr = oui_ptr->next;
            }
            memset(oui_ptr->id, 0x00, sizeof(oui_ptr->id));
            memset(oui_ptr->manuf, 0x00, sizeof(oui_ptr->manuf));
            snprintf(oui_ptr->id,
                     sizeof(oui_ptr->id),
                     "%c%c:%c%c:%c%c",
                     a[0],
                     a[1],
                     b[0],
                     b[1],
                     c[0],
                     c[1]);
            manuf = get_manufacturer_from_string(buffer);
            if (manuf != NULL)
            {
                snprintf(oui_ptr->manuf, sizeof(oui_ptr->manuf), "%s", manuf);
                free(manuf);
            }
            else
            {
                snprintf(oui_ptr->manuf, sizeof(oui_ptr->manuf), "Unknown");
            }
            if (oui_head == NULL) oui_head = oui_ptr;
            oui_ptr->next = NULL;
        }
    }

    fclose(fp);
    return (oui_head);
}
