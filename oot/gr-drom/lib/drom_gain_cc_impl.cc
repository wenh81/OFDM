/* -*- c++ -*- */
/*
 * Copyright 2018 drom.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "drom_gain_cc_impl.h"
#include "radiodrom.h"

namespace gr {
    namespace drom {

        drom_gain_cc::sptr

        drom_gain_cc::make(int symbol_length, float gain) {
            return gnuradio::get_initial_sptr
                (new drom_gain_cc_impl(symbol_length, gain));
        }

        /*
         * The private constructor
         */
         drom_gain_cc_impl::drom_gain_cc_impl (
             int symbol_length,
             float gain
         )
            : gr::block(
                "drom_gain_cc",
                gr::io_signature::make(1, 1, sizeof(gr_complex)),
                gr::io_signature::make(1, 1, sizeof(gr_complex))
            ),
            d_counter(0),
            d_symbol_length(symbol_length),
            d_gain(gain),
            d_prev(new gr_complex[symbol_length])
        {
            set_output_multiple(symbol_length);
        }


        /*
         * Our virtual destructor.
         */
         drom_gain_cc_impl::~drom_gain_cc_impl()
         {
         }

         void drom_gain_cc_impl::forecast (
             int noutput_items,
             gr_vector_int &ninput_items_required
         )
         {
             ninput_items_required[0] = noutput_items * 2;
         }

         int drom_gain_cc_impl::general_work (
             int noutput_items,
             gr_vector_int &ninput_items,
             gr_vector_const_void_star &input_items,
             gr_vector_void_star &output_items
         )
         {
             const gr_complex *in = (const gr_complex *) input_items[0];
             gr_complex *out = (gr_complex *) output_items[0];

             // radiodrom_chunk_gain(in, out, noutput_items, d_gain, d_symbol_length);

             radiodrom_chunk_cmul_conj(in, out, d_prev, noutput_items, d_symbol_length);

             consume_each (noutput_items);

             // Tell runtime system how many output items we produced.
             return noutput_items;
         }

     } /* namespace drom */
} /* namespace gr */
