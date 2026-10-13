#pragma once
#include <stdint.h>
// "Ponto certo": a marker sweeps the bar and the player stops it inside the target. Three rounds, faster each time.
class MiniGame {
 public:
  static constexpr uint8_t kRounds = 3;
  static constexpr uint32_t kRevealMs = 800;
  void start(uint32_t now, uint32_t seed) {
    seed_ = seed; round_ = hits_ = 0; hit_ = false; phase_ = Phase::Playing; roundStart_ = now;
  }
  bool active() const { return phase_ != Phase::Idle && phase_ != Phase::Done; }
  bool finished() const { return phase_ == Phase::Done; }
  void stop() { phase_ = Phase::Idle; }
  bool revealing() const { return phase_ == Phase::Reveal; }
  bool lastHit() const { return hit_; }
  uint8_t round() const { return round_; }
  uint8_t hits() const { return hits_; }
  uint8_t cursor(uint32_t now) const {
    if (phase_ == Phase::Reveal) return stopped_;
    uint32_t period = periodMs(), t = uint32_t(now - roundStart_) % period, half = period / 2;
    return uint8_t(t < half ? t * 100 / half : (period - t) * 100 / half);
  }
  uint8_t target() const { return uint8_t(20 + ((seed_ >> (round_ * 8)) & 0xff) % 61); }
  uint8_t width() const { return uint8_t(12 - round_ * 2); }
  void press(uint32_t now) {
    if (phase_ != Phase::Playing) return;
    stopped_ = cursor(now); revealStart_ = now; phase_ = Phase::Reveal;
    uint8_t distance = stopped_ > target() ? stopped_ - target() : target() - stopped_;
    hit_ = distance <= width(); if (hit_) ++hits_;
  }
  void update(uint32_t now) {
    if (phase_ != Phase::Reveal || uint32_t(now - revealStart_) < kRevealMs) return;
    if (++round_ >= kRounds) { phase_ = Phase::Done; return; }
    phase_ = Phase::Playing; roundStart_ = now;
  }
 private:
  enum class Phase : uint8_t { Idle, Playing, Reveal, Done };
  uint32_t periodMs() const { return 1400 - round_ * 250; }
  Phase phase_ = Phase::Idle;
  uint32_t seed_ = 0, roundStart_ = 0, revealStart_ = 0;
  uint8_t round_ = 0, hits_ = 0, stopped_ = 0;
  bool hit_ = false;
};
