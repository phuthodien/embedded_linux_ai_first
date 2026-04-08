# AD-001: Dual Sensor Strategy

**Status:** Accepted
**Date:** 2026-02-04

## Context

Cần support nhiều kernel versions khác nhau.

## Decision

Sử dụng 2 loại sensor:
- **eBPF** cho kernel >= 5.8 (primary)
- **Kernel Module** cho kernel < 5.8 (fallback)

## Rationale

- eBPF là xu hướng của industry (Microsoft Defender, CrowdStrike đều dùng)
- eBPF performance tốt hơn Auditd 15-19%
- Kernel module cần cho legacy systems (RHEL 6/7)
- Không dùng Auditd vì các EDR thương mại đã deprecated

## Consequences

- Cần maintain 2 codebases cho sensor
- Team cần kernel development expertise
