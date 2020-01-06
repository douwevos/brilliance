//package org.fpel;
//
//import static org.hamcrest.Matchers.equalTo;
//import static org.junit.Assert.assertThat;
//
//import org.junit.Test;
//
//public class TestSimpleExpression {
//
//    
//    @Test
//    public void shouldEvalRawText() {
//        Fpel fpel = new Fpel();
//        FpelExpression expression = fpel.compile("This is a simple text");
//        FpelContext context = new FpelContext();
//        String result = expression.eval(context);
//        assertThat(result, equalTo("This is a simple text"));
//    }
//
//    @Test
//    public void shouldEvalSingleProperty() {
//        Fpel fpel = new Fpel();
//        FpelExpression expression = fpel.compile("This is a ${var} text");
//        FpelContext context = new FpelContext();
//        context.put("var", "simple");
//        String result = expression.eval(context);
//        assertThat(result, equalTo("This is a simple text"));
//    }
//
//    @Test
//    public void shouldEvalArrayProperty() {
//        Fpel fpel = new Fpel();
//        FpelExpression expression = fpel.compile("For ${var[0]} this is a ${var[1]} text");
//        FpelContext context = new FpelContext();
//        String ar[] = new String[] { "foo", "bar"};
//        context.put("var", ar);
//        String result = expression.eval(context);
//        assertThat(result, equalTo("For foo this is a bar text"));
//    }
//
//    @Test
//    public void shouldEvalArrayPropertyThenFunction() {
//        Fpel fpel = new Fpel();
//        FpelExpression expression = fpel.compile("For ${var[1].substring(2,5)}");
//        FpelContext context = new FpelContext();
//        String ar[] = new String[] { "foo", "BigCompanoes"};
//        context.put("var", ar);
//        String result = expression.eval(context);
//        assertThat(result, equalTo("For gCo"));
//    }
//
//    @Test
//    public void shouldEvalPropertyDotName() {
//        Fpel fpel = new Fpel();
//        FpelExpression expression = fpel.compile("City ${point.nameWithRegion}");
//        FpelContext context = new FpelContext();
//        context.put("point", new Stop());
//        String result = expression.eval(context);
//        assertThat(result, equalTo("City Hamburg"));
//    }
//
//    @Test
//    public void shouldEvalTwoExpression() {
//        Fpel fpel = new Fpel();
//        FpelExpression expression = fpel.compile("City #{point.nameWithRegion} #{point.hashCode()}");
//        FpelContext context = new FpelContext();
//        context.put("point", new Stop());
//        String result = expression.eval(context);
//        assertThat(result, equalTo("City Hamburg"));
//    }
//
//    
//    @Test
//    public void shouldEvalPropertyDotGetName() {
//        Fpel fpel = new Fpel();
//        FpelExpression expression = fpel.compile("City ${point.nameWithRegionA}");
//        FpelContext context = new FpelContext();
//        context.put("point", new Stop());
//        String result = expression.eval(context);
//        assertThat(result, equalTo("City HamburgA"));
//    }
//
//
//    @Test
//    public void shouldEvalPropertyDotGetNameWithArgs() {
//        Fpel fpel = new Fpel();
//        FpelExpression expression = fpel.compile("City ${point.getNameWithRegionB(4)}");
//        FpelContext context = new FpelContext();
//        context.put("point", new Stop());
//        String result = expression.eval(context);
//        assertThat(result, equalTo("City Hamb"));
//    }
//
//
//    @Test
//    public void shouldEvalLiteralString() {
//        Fpel fpel = new Fpel();
//        FpelExpression expression = fpel.compile("City ${'hela'}");
//        FpelContext context = new FpelContext();
//        context.put("point", new Stop());
//        String result = expression.eval(context);
//        assertThat(result, equalTo("City hela"));
//    }
//
//
//    @Test
//    public void shouldEvalLiteralConcat() {
//        Fpel fpel = new Fpel();
//        FpelExpression expression = fpel.compile("City ${'hela'.concat('hola')}");
//        FpelContext context = new FpelContext();
//        context.put("point", new Stop());
//        String result = expression.eval(context);
//        assertThat(result, equalTo("City helahola"));
//    }
//
//    @Test
//    public void shouldEvalWrapFunction() throws NoSuchMethodException, SecurityException {
//        Fpel fpel = new Fpel();
//        FpelExpression expression = fpel.compile("City ${wrap('hela','hola')}");
//        FpelContext context = new FpelContext();
//        context.definedFunction("wrap", TestSimpleExpression.class.getDeclaredMethod("wrap", String.class, String.class));
//        context.put("point", new Stop());
//        String result = expression.eval(context);
//        assertThat(result, equalTo("City hela, hola"));
//    }
//
//    @Test
//    public void shouldEvalWrapFunctionNullA() throws NoSuchMethodException, SecurityException {
//        Fpel fpel = new Fpel();
//        FpelExpression expression = fpel.compile("City ${wrap(null,'hola')}");
//        FpelContext context = new FpelContext();
//        context.definedFunction("wrap", TestSimpleExpression.class.getDeclaredMethod("wrap", String.class, String.class));
//        context.put("point", new Stop());
//        String result = expression.eval(context);
//        assertThat(result, equalTo("City hola"));
//    }
//
//    @Test
//    public void shouldEvalWrapFunctionNullB() throws NoSuchMethodException, SecurityException {
//        Fpel fpel = new Fpel();
//        FpelExpression expression = fpel.compile("City ${wrap('holx', null)}");
//        FpelContext context = new FpelContext();
//        context.definedFunction("wrap", TestSimpleExpression.class.getDeclaredMethod("wrap", String.class, String.class));
//        context.put("point", new Stop());
//        String result = expression.eval(context);
//        assertThat(result, equalTo("City holx"));
//    }
//
//    
//    @Test
//    public void shouldEvalEq() throws NoSuchMethodException, SecurityException {
//        Fpel fpel = new Fpel();
//        FpelExpression expression = fpel.compile("City ${'holx' eq 'holx')}");
//        FpelContext context = new FpelContext();
//        context.definedFunction("wrap", TestSimpleExpression.class.getDeclaredMethod("wrap", String.class, String.class));
//        context.put("point", new Stop());
//        String result = expression.eval(context);
//        assertThat(result, equalTo("City true"));
//    }
//
//    @Test
//    public void shouldEvalConditional() throws NoSuchMethodException, SecurityException {
//        Fpel fpel = new Fpel();
//        FpelExpression expression = fpel.compile("City ${'holx' eq 'holx' ? 'waar' : 'onwaar'}");
//        FpelContext context = new FpelContext();
//        context.definedFunction("wrap", TestSimpleExpression.class.getDeclaredMethod("wrap", String.class, String.class));
//        context.put("point", new Stop());
//        String result = expression.eval(context);
//        assertThat(result, equalTo("City waar"));
//    }
//
//    @Test
//    public void shouldEvalConditionalWithParent() throws NoSuchMethodException, SecurityException {
//        Fpel fpel = new Fpel();
//        FpelExpression expression = fpel.compile("City ${'holx' eq 'holx' ? ('b' eq 'bc' ? 'mooi' : 'waar') : 'onwaar'}");
//        FpelContext context = new FpelContext();
//        context.definedFunction("wrap", TestSimpleExpression.class.getDeclaredMethod("wrap", String.class, String.class));
//        context.put("point", new Stop());
//        String result = expression.eval(context);
//        assertThat(result, equalTo("City waar"));
//    }
//
//
//    @Test
//    public void shouldEvalLessThenConditional() throws NoSuchMethodException, SecurityException {
//        Fpel fpel = new Fpel();
//        FpelExpression expression = fpel.compile("City ${20 lt 30 ? 'waar' : 'onwaar'}");
//        FpelContext context = new FpelContext();
//        String result = expression.eval(context);
//        assertThat(result, equalTo("City waar"));
//    }
//
//    
//    public static class Stop {
//        public String nameWithRegion ="Hamburg";
//
//        public String getNameWithRegionA() {
//            return "HamburgA";
//        }
//
//        public String getNameWithRegionB(int length) {
//            return "HamburgB".substring(0,length);
//        }
//        
//        @Override
//        public int hashCode() {
//            return super.hashCode()+20;
//        }
//    }
//    
//    public static String wrap(String a, String b) {
//        if (a==null) {
//            if (b==null) {
//                return "";
//            }
//            return b;
//        } else if (b==null) {
//            return a;
//        }
//        return a+", "+b;
//    }
//    
//}
