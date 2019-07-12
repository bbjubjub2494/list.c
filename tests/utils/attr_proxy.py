from __future__ import annotations

from typing import TypeVar, Generic, Any

S = TypeVar('S')
class AttrProxy(Generic[S]):
    """Proxy that directs missed attribute accesses to another object.

    This class is a designed as a mix-in. *The subject* refers to the target of
    the attribute accesses.

    :param S subject: target of the redirected operations.
    :param args: passed thru to superclass constructor
    :param kwargs: passed thru to superclass constructor
    """
    def __init__(self, subject: S, *args, **kwargs) -> None:
        AttrProxy.raw_setattr(self, '_subject', subject)
        super().__init__(*args, **kwargs)

    def __getattr__(self, attr_name: str) -> Any:
        """Fetch an attribute from the subject.

        :param str attr_name: name of the attribute.
        :returns: The value of the attribute on the subject.
        """
        return getattr(self._subject, attr_name)

    def __setattr__(self, attr_name: str, new_value: Any) -> None:
        """Assign into an attribute of the subject.

        :param str attr_name: name of the attribute.
        :param Any new_value: value for the attribute.
        """
        return setattr(self._subject, attr_name, new_value)

    @staticmethod
    def unwrap(proxy: AttrProxy[S]) -> S:
        """Allow recovering the subject of the redirection.

        :param AttrProxy[S] proxy: an object with :any:`AttrProxy`.
        :returns: the object that receives attribute access operations for ``proxy``.
        """
        return proxy._subject

    @staticmethod
    def raw_setattr(proxy: AttrProxy[S], attr_name: str, new_value: Any) -> None:
        """Set an attribute without redirecting.

        This is needed to initialize instance attributes on the object.

        :param AttrProxy[S] proxy: an object with :any:`AttrProxy`.
        :param str attr_name: name of the attribute.
        :param Any new_value: value for the attribute.
        """
        super(AttrProxy, proxy).__setattr__(attr_name, new_value)
