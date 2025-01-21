typedef struct {
	Slice name;
	struct _AstType *type;
} AstStructMember;

typedef struct {
	AstStructMember *members;
} AstStructType;
