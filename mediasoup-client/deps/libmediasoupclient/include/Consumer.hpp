#ifndef MSC_CONSUMER_HPP
#define MSC_CONSUMER_HPP

#include <json.hpp>
#include <api/media_stream_interface.h> // webrtc::MediaStreamTrackInterface
#include <api/rtp_receiver_interface.h> // webrtc::RtpReceiverInterface
#include <string>
#include <api/call/audio_sink.h>

namespace mediasoupclient
{
	// Fast forward declarations.
	class Transport;
	class RecvTransport;

	class Consumer : webrtc::AudioTrackSinkInterface
	{
	public:
		class PrivateListener
		{
		public:
			virtual void OnClose(Consumer* consumer)                    = 0;
			virtual nlohmann::json OnGetStats(const Consumer* consumer) = 0;
		};

		/* Public Listener API */
		class Listener
		{
		public:
			virtual void OnTransportClose(Consumer* consumer) = 0;
		};

	private:
		Consumer(
		  PrivateListener* privateListener,
		  Listener* listener,
		  const std::string& id,
		  const std::string& localId,
		  const std::string& producerId,
		  webrtc::RtpReceiverInterface* rtpReceiver,
		  webrtc::MediaStreamTrackInterface* track,
		  const nlohmann::json& rtpParameters,
		  const nlohmann::json& appData);


	public:
		const std::string& GetId() const;
		const std::string& GetLocalId() const;
		const std::string& GetProducerId() const;
		bool IsClosed() const;
		const std::string GetKind() const;
		webrtc::RtpReceiverInterface* GetRtpReceiver() const;
		webrtc::MediaStreamTrackInterface* GetTrack() const;
		const nlohmann::json& GetRtpParameters() const;
		bool IsPaused() const;
		nlohmann::json& GetAppData();
		void Close();
		nlohmann::json GetStats() const;
		void Pause();
		void Resume();

		float GetRmsSignalLevel() const { return rmsSignalLevel; }

		void OnData(const void *audio_data, int bits_per_sample, int sample_rate, size_t number_of_channels, size_t number_of_frames);

	private:
		void TransportClosed();

		// RecvTransport will create instances and call private member TransporClosed.
		friend RecvTransport;

	private:
		// PrivateListener instance.
		PrivateListener* privateListener;
		// Public Listener instance.
		Listener* listener;
		// Id.
		std::string id;
		// localId.
		std::string localId;
		// Producer Id.
		std::string producerId;
		// Closed flag.
		bool closed{ false };
		// Associated RTCRtpReceiver.
		webrtc::RtpReceiverInterface* rtpReceiver{ nullptr };
		// Local track.
		webrtc::MediaStreamTrackInterface* track{ nullptr };

		// RTP parameters.
		nlohmann::json rtpParameters;
		// Paused flag.
		bool paused{ false };
		// App custom data.
		nlohmann::json appData{};

		// RMS signal level of last sample window in decibels
		float rmsSignalLevel = FLT_MIN;
	};
} // namespace mediasoupclient

#endif
