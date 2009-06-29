#pragma once

template <class TDataType> class CStackItem {
public:
	CStackItem *next;
	TDataType data;
};

template <class TDataType> class CStack {
public:
	CStackItem<TDataType> *cur;

	void Push(TDataType newData) {
		CStackItem<TDataType> *p = new CStackItem<TDataType>;
		p->data = newData;
		p->next = this->cur;
		this->cur = p;
	}
	void Pop() {
		if (!this->cur) return;
		CStackItem<TDataType> *p = cur;
		this->cur = this->cur->next;
		delete p;
	}
	void Free() {
		if (!this->cur) return;
		CStackItem<TDataType> *p = this->cur;
		while (p) {
			this->cur = this->cur->next;
			delete p;
			p = this->cur;
		}
	}
	TDataType Cur() {
		return this->cur->data;
	}
	operator TDataType() {
		return this->cur->data;
	}
	BOOL IsEmpty() {
		return !this->cur;
	}

	CStack() {cur = NULL;}
	~CStack() {Free();}
};
