/*
 * Copyright 2021-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file audio.h
 * @brief Audio API.
 */

#ifndef __MUTEKI_AUDIO_H__
#define __MUTEKI_AUDIO_H__

#include "common.h"
#include "devio.h"
#include "threading.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief PCM codec direction/mode.
 */
enum pcm_direction_e {
    /**
     * @brief Default direction. Usually #DIRECTION_OUT.
     */
    DIRECTION_DEFAULT,
    /**
     * @brief Output/playback mode.
     */
    DIRECTION_OUT,
    /**
     * @brief Input/recording mode.
     */
    DIRECTION_IN,
};

/**
 * @brief Sample format.
 */
enum pcm_format_e {
    /**
     * @brief Use the default format. Usually #FORMAT_PCM_STEREO.
     */
    FORMAT_AUTO = -1,
    /**
     * @brief PCM mono.
     */
    FORMAT_PCM_MONO = 1,
    /**
     * @brief PCM stereo.
     */
    FORMAT_PCM_STEREO = 3,
};

/**
 * @brief Use the default sample rate. Usually this is equivalent to 44100Hz.
 */
const int SAMPLE_RATE_AUTO = -1;

struct device_service_pcm_s;
struct pcm_config_s;
struct pcm_codec_context_s;
struct pcm_state_s;
struct pcm_frame_s;
struct pcm_decoder_buffer_s;

typedef struct device_service_pcm_s device_service_pcm_t;
typedef struct pcm_config_s pcm_config_t;
typedef struct pcm_codec_context_s pcm_codec_context_t;
typedef struct pcm_state_s pcm_state_t;
typedef struct pcm_frame_s pcm_frame_t;
typedef struct pcm_decoder_buffer_s pcm_decoder_buffer_t;

typedef void (*pcm_codec_func_t)(device_service_pcm_t *pcm, pcm_decoder_buffer_t *src, pcm_decoder_buffer_t *dest);

/**
 * @brief The PCM codec configuration.
 */
struct pcm_config_s {
    /**
     * @brief Unknown. Initializes to 0.
     */
    int unk_0x0;
    /**
     * @brief Unknown. Initializes to 0.
     */
    int unk_0x4;
    /**
     * @brief Current codec function in effect.
     */
    pcm_codec_func_t codec;
    int unk_0xc; // initialized to 0
    /**
     * @brief Sample rate in Hz.
     * @details Defaults to 44100 on at least BA742.
     */
    int sample_rate;
    /**
     * @brief Direction setting supplied to `direction` parameter during descriptor creation.
     * @details Defaults to ::DIRECTION_OUT.
     */
    int default_direction;
    /**
     * @brief Audio data frame format.
     * @details Defaults to ::FORMAT_PCM_STEREO.
     */
    int format;
    /**
     * @brief Current direction setting in effect.
     * @details Defaults to ::DIRECTION_OUT.
     */
    int current_direction;
    /**
     * @brief Unknown. Initializes to 0.
     */
    int unk_0x20;
    /**
     * @brief Unknown.
     */
    void *unk_0x24;
};

/**
 * @brief The PCM codec context.
 */
struct pcm_codec_context_s {
    /**
     * @brief The PCM service.
     */
    device_service_pcm_t *service;
    /**
     * @brief Unknown. Initializes to 0.
     */
    int unk_0x4;
    /**
     * @brief Configuration applied to this context.
     */
    pcm_config_t config;
};

struct device_service_pcm_s {
    device_service_t common;
    int unk_0x4c;
    int (*init)(device_service_pcm_t *);
    int (*reset)(device_service_pcm_t *);
    int (*fini)(device_service_pcm_t *);
    int unk_0x5c;
    pcm_config_t config;
    pcm_state_t *state;
    int unk_0x8c;
};

struct pcm_state_s {
    pcm_frame_t **frames;
    int unk_0x4;
    int unk_0x8;
    size_t size; /* Must be 2**n */
    int unk_0x10;
    size_t max_frames;
    size_t frame_queue_tip;
    bxc_cs_t cs;
};

/**
 * @brief PCM frame data structure. Used internally.
 */
struct pcm_frame_s {
    /**
     * @brief Pointer to buffer.
     */
    void *buffer;
    /**
     * @brief Size of the buffer.
     */
    size_t size;
    /**
     * @brief Size pending transfer.
     * @todo Verify.
     */
    size_t pending_size;
    /**
     * @brief Transfer state.
     * @todo Verify.
     */
    unsigned int tx_state;
};

/**
 * @brief Codec buffer descriptor. Used Internally.
 */
struct pcm_decoder_buffer_s {
    /**
     * @brief Pointer to buffer.
     */
    void *buffer;
    /**
     * @brief Size of the buffer.
     */
    size_t size;
    /**
     * @brief Sample rate.
     */
    int sample_rate;
    /**
     * @brief Channels.
     */
    int channels;
};

/**
 * @brief Create PCM codec context.
 * @details
 * Use this to play/record audio in a streaming manner.
 *
 * This changes the configuration of the `\\?\PCM` service. More details TBA.
 *
 * The context object can be used directly, but it's recommended to open another descriptor using the CreateFile()
 * syscall, and use ReadFile() / WriteFile() / DeviceIoControl() to access the device.
 *
 * For example, below is a simple loop that uses both the PCM and the device IO syscalls together to play samples:
 *
 * @code{.c}
 * char sample[0x2000]; // Write audio data to this buffer periodically
 * size_t num_bytes, actual_bytes;
 * 
 * pcm_codec_context_t *pcmdesc = OpenPCMCodec(DIRECTION_OUT, SAMPLE_RATE_AUTO, FORMAT_AUTO);
 * if (pcmdesc == NULL) {
 *     // TODO fail
 * }
 *
 * devio_descriptor_t *pcmdev = CreateFile("\\\\?\\PCM", 0, 0, NULL, 3, 0, NULL);
 * if (pcmdev == NULL || pcmdev == DEVIO_DESC_INVALID) {
 *     ClosePCMCodec(pcmdesc);
 *     // TODO fail
 * }
 *
 * while (true) {
 *     // TODO populate the samples and set num_bytes to how many bytes to play
 *     WriteFile(pcmdev, sample, num_bytes, &actual_bytes, NULL);
 *     if (num_bytes != actual_bytes) {
 *         // TODO optionally report error
 *     }
 * }
 *
 * CloseHandle(pcmdev);
 * ClosePCMCodec(pcmdesc);
 * @endcode
 *
 * @x_syscall_num{0x10250}
 *
 * @param direction Direction. Can either be input (record) or output (playback).
 * @param sample_rate Sample rate.
 * @param format Sample format.
 * @return The context.
 * @see pcm_direction_e Predefined values for codec direction.
 * @see pcm_format_e Supported sample formats.
 */
extern pcm_codec_context_t *OpenPCMCodec(int direction, int sample_rate, int format);

/**
 * @brief Destroy a PCM codec context.
 * @x_syscall_num{0x10251}
 * @param ctx The context.
 */
extern void ClosePCMCodec(pcm_codec_context_t *ctx);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_AUDIO_H__
