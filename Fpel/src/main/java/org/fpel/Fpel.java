package org.fpel;

import java.util.ArrayList;
import java.util.List;

import org.fpel.FpelExpression.FpelExpressionBuilder;
import org.fpel.evaluable.FpelArrayAccess;
import org.fpel.evaluable.FpelConditional;
import org.fpel.evaluable.FpelEvalOutput;
import org.fpel.evaluable.FpelGetProperty;
import org.fpel.evaluable.FpelLiteral;
import org.fpel.evaluable.FpelMethodInvocation;
import org.fpel.evaluable.FpelRelational;

import lombok.AllArgsConstructor;

public class Fpel {

    public FpelExpression compile(String expression) {
        FpelExpressionBuilder expressionBuilder = FpelExpression.builder();

        int offset = 0;
        int idxMa = expression.indexOf("#{");
        int idxMb = expression.indexOf("${");
        while (true) {
            int idxStart = 0;
            if (idxMa == -1) {
                if (idxMb == -1) {
                    break;
                }
                idxStart = idxMb;
            }
            else if (idxMb == -1) {
                idxStart = idxMa;
            }
            else {
                idxStart = idxMa < idxMb ? idxMa : idxMb;
            }

            int idxEnd = expression.indexOf("}", idxStart + 2);
            if (idxEnd >= 0) {

                if (idxStart > offset) {
                    expressionBuilder.addPart(new FpelRawOuput(offset, expression.substring(offset, idxStart)));
                }

                String sub = expression.substring(idxStart + 2, idxEnd);
                FpelEvaluable part = parsePart(sub);
                if (part==null) {
                	expressionBuilder.addPart(new FpelEvalOutput(idxStart));
                } else {
                	expressionBuilder.addPart(new FpelEvalOutput(idxStart, part));
                }

                offset = idxEnd + 1;
            }
            else {
                idxEnd = idxStart + 2;
            }

            if (idxStart == idxMa) {
                idxMa = expression.indexOf("#{", idxEnd);
            }
            else {
                idxMb = expression.indexOf("${", idxEnd);
            }

        }
        if (offset < expression.length()) {
            expressionBuilder.addPart(new FpelRawOuput(offset, expression.substring(offset, expression.length())));
        }

        return expressionBuilder.build();
    }

    private FpelEvaluable parsePart(String fpelText) {
        ParseContext parseContext = new ParseContext(fpelText);
        return parseSuperExpression(parseContext);

    }

    private FpelEvaluable parseSuperExpression(ParseContext parseContext) {
        return parseConditionalExpression(parseContext);
    }
    
    private FpelEvaluable parseConditionalExpression(ParseContext parseContext) {
        FpelEvaluable result = parseRelationalExpression(parseContext);
        FpelStackEntry mainEntry = parseContext.getEntry();
        if (mainEntry.token.getType()==FpelTokenType.CONDITIONAL) {
            parseContext.nextEntry();
            FpelEvaluable trueExpr = parseSuperExpression(parseContext);
            parseContext.expect(FpelTokenType.COLON);
            FpelEvaluable falseExpr = parseSuperExpression(parseContext);
            
            result = new FpelConditional(result, trueExpr, falseExpr);
            parseContext.reduce(5, result);
        }
        return result;
    }

    
    private FpelEvaluable parseRelationalExpression(ParseContext parseContext) {
        FpelEvaluable left = parseExpression(parseContext);
        FpelStackEntry mainEntry = parseContext.getEntry();
        switch(mainEntry.token.getType()) {
            case EOT : 
                return left;
            case EQ :
            case NE :
            case LT :
            case GT :
            case LE :
            case GE :
                parseContext.nextEntry();
                FpelEvaluable right = parseExpression(parseContext);
                FpelEvaluable result = new FpelRelational(left, right, mainEntry.token.getType());
                left = parseContext.reduce(3, result);
                break;
        }
        return left;
    }

    
    private FpelEvaluable parseExpression(ParseContext parseContext) {
        FpelStackEntry mainEntry = parseContext.getEntry();

        FpelEvaluable result = null;
        switch (mainEntry.token.getType()) {
            case EOT:
                return null;
            case NULL:
            case INTEGER:
                result = parseLiteral(parseContext);
                break;
            case STRING:
            case IDENTIFIER:
                result = parseInvocationOrFieldAccess(parseContext);
                break;
            case LEFT_PAREN:
                result = parseParenthizedExpression(parseContext);
                break;
        }
        return result;
    }

    private FpelEvaluable parseParenthizedExpression(ParseContext parseContext) {
        parseContext.expect(FpelTokenType.LEFT_PAREN);
        FpelEvaluable result = parseSuperExpression(parseContext);
        parseContext.expect(FpelTokenType.RIGHT_PAREN);
        return parseContext.reduce(3, result);
    }

    private FpelEvaluable parseInvocationOrFieldAccess(ParseContext parseContext) {
        return parseName(parseContext);
    }

    /**
     * name = identifier | name DOT identifier
     */
    private FpelEvaluable parseName(ParseContext parseContext) {
        FpelEvaluable result = null;

        FpelStackEntry dotEntry = null;
        while (true) {
            FpelEvaluable nextPart = parseDotPart(result, dotEntry, parseContext);
            if (nextPart == null) {
            	break;
            }
            result = nextPart;

            FpelStackEntry followEntry = parseContext.getEntry();
            while(followEntry.token.getType()==FpelTokenType.LEFT_SQ_BRACK) {
                result = parseArrayAccess(parseContext);
                followEntry = parseContext.getEntry();
            }
            
            dotEntry = parseContext.getEntry();
            if (dotEntry.token.getType() != FpelTokenType.DOT) {
                break;
            }
            parseContext.nextEntry();

        }

        return result;
    }


    private FpelEvaluable parseDotPart(FpelEvaluable leftHand, FpelStackEntry dotEntry, ParseContext parseContext) {
        FpelStackEntry idEntry = parseContext.getEntry();
        if (idEntry.token.getType() == FpelTokenType.IDENTIFIER) {
            FpelStackEntry nextEntry = parseContext.nextEntry();
            if (nextEntry.token.getType() == FpelTokenType.LEFT_PAREN) {
            	return parseMethodInvocation(parseContext, leftHand, idEntry);
            }
            return parseContext.reduce(dotEntry==null ? 1 : 3, new FpelGetProperty(leftHand, idEntry.token));
        } else if (leftHand!=null ) { 
        	// TODO ERROR: <evaluable> DOT !IDENTIFIER     expected IDENTIFIER
        	int right = dotEntry.token.getRight();
        	FpelToken t = new FpelToken(FpelTokenType.IDENTIFIER, "", right, right);
            return parseContext.reduce(1, new FpelGetProperty(leftHand, t));
        	
        }
        if (leftHand==null) {
            if (idEntry.token.getType() == FpelTokenType.STRING) {
                return parseLiteral(parseContext);
            }
        }

        return null;
    }

    private FpelEvaluable parseMethodInvocation(ParseContext parseContext, FpelEvaluable leftHand, FpelStackEntry idEntry) {
        parseContext.nextEntry();
        List<FpelEvaluable> arguments = parseArguments(parseContext);
        FpelStackEntry entry = parseContext.getEntry();
        if (entry.token.getType()==FpelTokenType.RIGHT_PAREN) {
            parseContext.nextEntry();
        } else {
            throw new RuntimeException(") expected");
        }
        
        /* ID LPAREN ARGS[n] RPAREN */
        return parseContext.reduce(3+arguments.size(), new FpelMethodInvocation(leftHand, idEntry.token, arguments));
    }

    private List<FpelEvaluable> parseArguments(ParseContext parseContext) {
        
        List<FpelEvaluable> result = new ArrayList<>();
        
        while(true) {
            FpelEvaluable expression = parseExpression(parseContext);
            if (expression==null) {
                FpelStackEntry commaEntry = parseContext.getEntry();
                if (commaEntry.token.getType()==FpelTokenType.COMMA) {
//                	commaEntry.token.markError("expected an expression or a right-parenthesis");
                	parseContext.nextEntry();
                    continue;
                }
                break;
            }
            result.add(expression);
            FpelStackEntry commaEntry = parseContext.getEntry();
            if (commaEntry.token.getType()!=FpelTokenType.COMMA) {
                break;
            }
            parseContext.nextEntry();
        }
        return result;
    }

    private FpelEvaluable parseLiteral(ParseContext parseContext) {
        FpelStackEntry entry = parseContext.getEntry();
        entry.part = new FpelLiteral(entry.token);
        parseContext.nextEntry();
        return entry.part;
    }


    private FpelEvaluable parseArrayAccess(ParseContext parseContext) {
        FpelStackEntry idStackEntry = parseContext.getBackEntry(0);
        FpelToken tokenLeftSq = parseContext.getEntry().token;
        FpelStackEntry offsetStackEntry = parseContext.nextEntry();
        FpelEvaluable fieldArrayOffset = parseExpression(parseContext);
        offsetStackEntry.part = fieldArrayOffset;
        if (fieldArrayOffset == null) {
        	// TODO mark next token as array... expected a field-array-offset
        }
        
        FpelStackEntry closeEntry = parseContext.getEntry();
        if (closeEntry.token.getType() != FpelTokenType.RIGHT_SQ_BRACK) {
            // throw new ParseError();
        }
        FpelToken tokenRightSq = parseContext.getEntry().token;
        parseContext.nextEntry();

        FpelEvaluable fielEvaluable = idStackEntry.part;

        return parseContext.reduce(fieldArrayOffset==null ? 3 : 4, new FpelArrayAccess(fielEvaluable, tokenLeftSq, fieldArrayOffset, tokenRightSq));
    }

    static class ParseContext {

        private int stackOffset = 0;
        public List<FpelStackEntry> stack = new ArrayList<>();
        FpelTokenizer tokenizer;

        public ParseContext(String fpelText) {
            tokenizer = new FpelTokenizer(fpelText);
        }

        public void expect(FpelTokenType expect) {
            FpelStackEntry current = getEntry();
            if (current.token.getType()!=expect) {
                throw new RuntimeException("expected "+expect);
            }
            stackOffset++;
            
        }

        public FpelStackEntry getLookahead(int lah) {
            return getEntryAt(stackOffset + lah);
        }

        public FpelEvaluable reduce(int i, FpelEvaluable fpelArrayAccess) {
            for (int idx = 1; idx < i; idx++) {
                stack.remove(stackOffset - 1);
                stackOffset--;
            }
            FpelStackEntry entry = stack.get(stackOffset - 1);
            entry.part = fpelArrayAccess;
            return fpelArrayAccess;
        }

        public FpelStackEntry getBackEntry(int topOffset) {
            return stack.get(stackOffset - 1 - topOffset);
        }

        public FpelStackEntry nextEntry() {
            stackOffset++;
            return getEntry();
        }

        public FpelStackEntry getEntry() {
            if (stackOffset >= stack.size()) {
                stack.add(new FpelStackEntry(tokenizer.nextToken(), null));
            }
            return stack.get(stackOffset);
        }

        public FpelStackEntry getEntryAt(int offset) {
            while (offset >= stack.size()) {
                stack.add(new FpelStackEntry(tokenizer.nextToken(), null));
            }
            return stack.get(offset);
        }

    }

    @AllArgsConstructor
    private static class FpelStackEntry {

        public FpelToken token;
        public FpelEvaluable part;

    }

}
