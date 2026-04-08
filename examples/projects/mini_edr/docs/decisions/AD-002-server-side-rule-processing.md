# AD-002: Server-Side Rule Processing

**Status:** Accepted
**Date:** 2026-02-04

## Context

Quyết định nơi thực hiện MITRE rule matching.

## Decision

Tất cả rule matching thực hiện trên server, agent chỉ thu thập và gửi raw events.

## Rationale

- Giảm complexity cho agent
- Dễ update rules mà không cần update agent
- Server có nhiều resource hơn để correlate events từ nhiều endpoints

## Consequences

- Network bandwidth cao hơn (gửi tất cả events)
- Cần offline buffer khi mất kết nối
