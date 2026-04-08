# AD-003: gRPC for Communication

**Status:** Accepted
**Date:** 2026-02-04

## Context

Chọn protocol giao tiếp giữa agent và server.

## Decision

Sử dụng gRPC với TLS.

## Rationale

- Binary protocol, hiệu quả hơn REST/JSON
- Hỗ trợ streaming (phù hợp cho event stream)
- Strongly typed với Protocol Buffers
- Hỗ trợ bidirectional (server có thể gửi commands)
