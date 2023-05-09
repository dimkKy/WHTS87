#pragma once

template<class S, class O>
bool WHTS87Utils::IsIn(const S& subject, const O* other)
{
	return &subject == other;
}

template<class S, class O, class ...Os>
bool WHTS87Utils::IsIn(const S& subject, const O* other, const Os * ...others)
{
	return &subject == other ? true : IsIn(subject, others...);
}