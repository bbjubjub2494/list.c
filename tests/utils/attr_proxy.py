from __future__ import annotations

from typing import TypeVar, Generic, Any

S = TypeVar('S')
class AttrProxy(Generic[S]):
    def __init__(self, subject: S, *args, **kwargs) -> None:
        AttrProxy.raw_setattr(self, '_subject', subject)
        super().__init__(*args, **kwargs)

    def __getattr__(self, attr_name: str) -> Any:
        return getattr(self._subject, attr_name)

    def __setattr__(self, attr_name: str, new_value: Any) -> None:
        return setattr(self._subject, attr_name, new_value)

    @staticmethod
    def unwrap(proxy: AttrProxy[S]) -> S:
        return proxy._subject

    @staticmethod
    def raw_setattr(proxy: AttrProxy[S], attr_name: str, new_value: Any) -> None:
        super(AttrProxy, proxy).__setattr__(attr_name, new_value)
